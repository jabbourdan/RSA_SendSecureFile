#include "NetworkHandler.h"


NetworkHandler::NetworkHandler(const std::string& client_name)
	: requests_generator(client_name)
{
}

bool NetworkHandler::serverResponse(boost::asio::ip::tcp::socket& socket, std::vector<std::string>& response_data)
{

	boost::asio::streambuf response_buffer;
	boost::system::error_code error;

	boost::asio::read_until(socket, response_buffer, '\0', error);

	if (error)
	{
		std::cerr << "Error reading server response: " << error.message() << std::endl;
		return false;
	}

	std::string response_string = boost::asio::buffer_cast<const char*>(response_buffer.data());

	std::cout << "RESPONSE RECEIVED: " << response_string << std::endl;

	response_data.clear(); // remove previous data
	response_data = string_handler.splitString(response_string, ':');

	// Clear and release the response_buffer's memory
	response_buffer.consume(response_buffer.size());

	return true;
}

bool NetworkHandler::sendRequest(boost::asio::ip::tcp::socket& socket, uint16_t request_code)
{
	Request request_to_server;
	this->requests_generator.generateRequest(request_to_server, request_code);

	size_t retry_count = 0;
	while (retry_count < NETWORK::MAX_REQUEST_RETRIES) {
		try {
			boost::asio::write(socket, boost::asio::buffer(string_handler.serializeRequest(request_to_server)));
			std::cout << "REQUEST SENT: " << string_handler.serializeRequest(request_to_server) << std::endl;
			return true; // Exit loop if request was successfully sent
		}
		catch (const boost::system::system_error& e) {
			std::cerr << "ERROR: " << e.what() << " RETRYING..." << std::endl;
			retry_count++;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for 1 second before retrying
		}
	}
	std::cerr << "SEND ERROR: Failed to send request after " << NETWORK::MAX_REQUEST_RETRIES << " attempts" << std::endl;
	return false;
}

bool NetworkHandler::sendRegOrReconnectRequest(boost::asio::ip::tcp::socket& socket, const std::string& client_name, const std::string& client_uuid)
{

	uint16_t request_code;
	if (client_uuid.empty()) // registration request
	{
		this->requests_generator.setClientID("UNASSIGNED_UUID"); // this will be assigned from server on next reesponse
		request_code = REQUEST::REGISTRATION;
	}
	else // reconnect request
	{
		this->requests_generator.setClientID(client_uuid);
		request_code = REQUEST::RECONNECT;
	}

	this->requests_generator.setClientName(client_name);


	bool is_send_success = sendRequest(socket, request_code);
	return is_send_success;
}

bool NetworkHandler::sendPublicKeyRequest(boost::asio::ip::tcp::socket& socket, EncryptionHandler& encryption_handler, const std::string& client_uuid)
{
	std::string public_key = encryption_handler.getPublicKey();

	this->requests_generator.setClientID(client_uuid);
	this->requests_generator.set_public_key(public_key);
	bool is_send_success = sendRequest(socket, REQUEST::KEYS_EXCHANGE);

	return is_send_success;
}

bool NetworkHandler::sendFileRequest(boost::asio::ip::tcp::socket& socket, const std::string& file_path, EncryptionHandler& encryption_handler, const std::string& aes_key, const std::vector<std::string>& response_data)
{
	try
	{
		FilesHandler filesHandlerObject;
		std::string file_name = filesHandlerObject.extractFileNameFromTransferInfo();
		CryptoPP::SecByteBlock aes_key_decoded = encryption_handler.decodeBase64Key(aes_key);
		std::string encrypted_file;
		encrypted_file = encryption_handler.encryptFile(file_path, aes_key_decoded);
		// Encode encrypted file as base64
		std::string base64_encrypted_file;
		CryptoPP::StringSource(encrypted_file, true,
			new CryptoPP::Base64Encoder(
				new CryptoPP::StringSink(base64_encrypted_file)));

		if (!base64_encrypted_file.empty() && base64_encrypted_file.back() == '\n')
			base64_encrypted_file.pop_back();

		uint32_t encrypted_file_size = base64_encrypted_file.length(); // each char is 1 byte, therefore encrypted size will be the size of this string


		this->requests_generator.setFileName(file_name);
		this->requests_generator.setFileContent(base64_encrypted_file);
		this->requests_generator.setFileSize(encrypted_file_size);
		bool is_send_success = sendRequest(socket, REQUEST::FILE_TRANSFER);
		return is_send_success;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception in sendFileRequest: " << e.what() << std::endl;
		return false;
	}
}

bool NetworkHandler::handleFileTransfer(boost::asio::ip::tcp::socket& socket, EncryptionHandler& encryption_handler, FilesHandler& files_handler,
	std::string& file_path, std::string& aes_key,
	std::vector<std::string>& response_data, size_t& file_transfer_attempts)
{
    std::string client_crc = files_handler.calculateCRC(file_path);
    std::string server_crc = response_data.at(6);

    if (client_crc == server_crc)
    {
        sendRequest(socket, REQUEST::VALID_CKSUM_FINISH);
        return true;
    }
    else if (file_transfer_attempts <= 3)
    {
        sendRequest(socket, REQUEST::INVALID_CKSUM_RESEND);
        if (!sendFileRequest(socket, file_path, encryption_handler, aes_key, response_data))
            return false;
        file_transfer_attempts++;
        return true;
    }
    else // 4th attempt to send the file failed.
    {
        sendRequest(socket, REQUEST::INVALID_CKSUM_FINISH);
        return false;
    }
	return false;
}