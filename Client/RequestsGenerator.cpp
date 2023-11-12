#include "RequestsGenerator.h"

RequestsGenerator::RequestsGenerator(const std::string& client_name) {
	// Limit the size of all string variables by initing them with '\0' char through max bytes length
	this->client_name = std::string(MAX_BYTES::CLIENT_NAME, '\0');
	this->client_uuid = std::string(MAX_BYTES::CLIENT_ID, '\0');
	this->public_key = std::string(MAX_BYTES::PUBLIC_KEY, '\0');
	this->file_name = std::string(MAX_BYTES::FILE_NAME, '\0');

	this->client_name.reserve(MAX_BYTES::CLIENT_NAME);
	this->client_uuid.reserve(MAX_BYTES::CLIENT_ID);
	this->public_key.reserve(MAX_BYTES::PUBLIC_KEY);
	this->file_name.reserve(MAX_BYTES::FILE_NAME);

	// Assign client_name
	this->client_name = client_name.c_str();
	// Assign temporary uuid (this will be shown only on 1st registration request call)
	this->client_uuid = "NEW_USER";
}

void RequestsGenerator::generateRequest(Request& request, size_t request_code) {

	// reset request's variables
	request = {}; 

	// set header
	request.client_uuid = this->client_uuid.c_str();
	request.version = CLIENT::VERSION;
	request.request_code = request_code;
	request.payload_size = 999; // add "uint32_t calculate_payload_size(size_t request_code);

	if (request_code == REQUEST::REGISTRATION)
		getRegRequestPayload(request);
	else if (request_code == REQUEST::KEYS_EXCHANGE)
		getKeyRequestPayload(request);
	else if (request_code == REQUEST::RECONNECT)
		getReconnectPayload(request);
	else if (request_code == REQUEST::FILE_TRANSFER)
		getFileRequestPayload(request);
	else if (request_code == REQUEST::VALID_CKSUM_FINISH)
		getValidCksumrequestPayload(request);
	else if (request_code == REQUEST::INVALID_CKSUM_RESEND)
		getInvalidCksumPayload(request);
	else if (request_code == REQUEST::INVALID_CKSUM_FINISH)
		getInvalidCksumRequestPayload(request);
}


void RequestsGenerator::getRegRequestPayload(Request& request) {
	request.client_name = this->client_name.c_str();
}

void RequestsGenerator::getKeyRequestPayload(Request& request) {
	request.client_name = this->client_name.c_str();
	request.public_key = this->public_key.c_str();
}

void RequestsGenerator::getReconnectPayload(Request& request) {
	request.client_name = this->client_name.c_str();
}

void RequestsGenerator::getFileRequestPayload(Request& request) {
	request.file_size = this->file_size;
	request.file_name = this->file_name.c_str();
	request.file_content = this->file_content.c_str();
}

void RequestsGenerator::getValidCksumrequestPayload(Request& request) {
	request.client_name = this->client_name.c_str();
}

void RequestsGenerator::getInvalidCksumPayload(Request& request) {
	request.client_name = this->client_name.c_str();
}

void RequestsGenerator::getInvalidCksumRequestPayload(Request& request) {
	request.client_name = this->client_name.c_str();
}

void RequestsGenerator::setClientID(const std::string &client_id)
{
	this->client_uuid = client_id;
}

void RequestsGenerator::setClientName(const std::string &client_name)
{
	this->client_name = client_name;
}

void RequestsGenerator::set_public_key(const std::string& public_key)
{
	this->public_key = public_key;
}

void RequestsGenerator::setFileSize(uint32_t file_size)
{
	this->file_size = file_size;
}


void RequestsGenerator::setFileName(const std::string& file_name)
{
	this->file_name = file_name;
}


void RequestsGenerator::setFileContent(const std::string& file_content)
{
	this->file_content = file_content;
}