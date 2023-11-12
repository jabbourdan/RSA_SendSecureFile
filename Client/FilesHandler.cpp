#include "FilesHandler.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool FilesHandler::initVariables(std::string& client_name, std::string& client_uuid, std::string& private_key, std::string& file_path, std::string& file_name)
{
    try
    {
        client_name = readClient();
        client_uuid = readClientID();
        private_key = readPrivateKey();
        file_path = readFilePath();
        
        file_name = extractFileNameFromTransferInfo();
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

std::string FilesHandler::extractFileNameFromTransferInfo() {
    std::string transferInfoFilePath = "./transfer.info"; 
    std::ifstream transferInfoFile(transferInfoFilePath);
	
    std::string fileName;

    if (transferInfoFile.is_open()) {
        std::string line;
        for (int i = 0; i < 3; ++i) {
            if (!std::getline(transferInfoFile, line)) {
                std::cerr << "Insufficient lines in transfer.info." << std::endl;
                return "";  // Return empty string if there are not enough lines
            }
        }

        fileName = line;  // The file name is in the third line of transfer.info
        transferInfoFile.close();
    } else {
        std::cerr << "Unable to open transfer.info file." << std::endl;
    }

    return fileName;
}

// Function to read client name from me.info or transfer.info
std::string FilesHandler::readClient() {
	std::string client_name;
	std::ifstream me_file(FILES::ME_FILE);

	if (fileExist(FILES::ME_FILE)) {
		// Read client name from me.info file
		std::getline(me_file, client_name);
	}
	else {
		// Read client name from transfer.info file
		std::ifstream transfer_file(FILES::TRANSFER_FILE);
		if (!transfer_file) {
			throw std::exception();
		}
		std::string line;
		std::getline(transfer_file, line); // Skip first line
		std::getline(transfer_file, client_name); // Read client name from second line
		if (client_name.empty()) {
			throw std::runtime_error("Client name not found in " + FILES::TRANSFER_FILE);
		}
	}

	return client_name;
}

// Function to read client uuid from me.info
std::string FilesHandler::readClientID() {
	std::ifstream me_file(FILES::ME_FILE);
	std::string client_uuid;

	if (fileExist(FILES::ME_FILE)) {
		std::getline(me_file, client_uuid); // skip client name
		std::getline(me_file, client_uuid);
	}

	return client_uuid;
}

// Function to read RSA key from me.info
std::string FilesHandler::readPrivateKey() {
	std::ifstream me_file(FILES::ME_FILE);
	std::string RSA_key;

	if (fileExist(FILES::ME_FILE)) {
		std::getline(me_file, RSA_key); // skip client name and uuid
		std::getline(me_file, RSA_key);
		std::getline(me_file, RSA_key);
	}

	return RSA_key;
}

// Check if file exists
bool FilesHandler::fileExist(const std::string& filename) {
	std::ifstream file(filename);
	if (file.good()) {
		file.close();
		return true;
	}
	return false;
}

// Creates me file
void FilesHandler::createME(std::string client_name, std::string client_uuid, std::string private_key) {
	std::ofstream me_info_file(FILES::ME_FILE);
	if (me_info_file.is_open()) {
		me_info_file << client_name << "\n";
		me_info_file << client_uuid << "\n";
		me_info_file << private_key << "\n";
		me_info_file.close();
		std::cout << "me.info file created successfully.\n";
	}
	else {
		std::cerr << "Unable to create me.info file.\n";
	}
}
std::string FilesHandler::readIPAddress()
{
	std::string ip_address;
	std::ifstream file(FILES::TRANSFER_FILE);
	if (!file)
	{
		throw std::runtime_error("Failed to open " + FILES::TRANSFER_FILE);
	}

	std::string line;
	std::getline(file, line);

	std::regex pattern("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d{1,5})$");
	std::smatch match;

	if (std::regex_search(line, match, pattern))
	{
		// Valid IP address and port
		ip_address = match[1];
	}
	else
	{
		// Invalid IP address and/or port
		std::cerr << "Invalid IP address and/or port number in " << FILES::TRANSFER_FILE << std::endl;
		ip_address = NETWORK::DEFAULT_IP;
	}

	// Validate IP address
	std::regex ip_pattern("^(\\d{1,3}\\.){3}\\d{1,3}$");
	if (!std::regex_match(ip_address, ip_pattern))
	{
		std::cerr << "Invalid IP address in " << FILES::TRANSFER_FILE << ", setting to " << NETWORK::DEFAULT_IP << std::endl;
		ip_address = NETWORK::DEFAULT_IP;
	}

	return ip_address;
}

// Function to read port from transfer.info file
std::string FilesHandler::readPort()
{
	std::string port;
	std::ifstream file(FILES::TRANSFER_FILE);
	if (!file)
	{
		throw std::runtime_error("Failed to open " + FILES::TRANSFER_FILE);
	}

	std::string line;
	std::getline(file, line);

	std::regex pattern("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d{1,5})$");
	std::smatch match;

	if (std::regex_search(line, match, pattern))
	{
		// Valid IP address and port
		port = match[2];
	}
	else
	{
		// Invalid IP address and/or port
		std::cerr << "Invalid IP address and/or port number in " << FILES::TRANSFER_FILE << std::endl;
		port = NETWORK::DEFAULT_PORT;
	}

	// Validate port number
	unsigned int port_num = std::stoul(port);
	if (port_num < NETWORK::MIN_PORT_NUMBER || port_num > NETWORK::MAX_PORT_NUMBER)
	{
		std::cerr << "Invalid port number in " << FILES::TRANSFER_FILE << ", setting to " << NETWORK::DEFAULT_PORT << std::endl;
		port = NETWORK::DEFAULT_PORT;
	}

	return port;
}
// Function to read filepath from transfer.info
std::string FilesHandler::readFilePath() {
	std::ifstream transfer_file(FILES::TRANSFER_FILE);
	std::string filepath;

	if (fileExist(FILES::TRANSFER_FILE)) {
		std::getline(transfer_file, filepath);
		std::getline(transfer_file, filepath);
		std::getline(transfer_file, filepath);
	}

	if (filepath.empty()) {
		throw std::runtime_error("Client name not found in " + FILES::TRANSFER_FILE);
	}

	return filepath;
}

std::string FilesHandler::calculateCRC(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return "";
    }

    boost::crc_32_type crc;
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)))
    {
        crc.process_bytes(buffer, sizeof(buffer));
    }
    crc.process_bytes(buffer, file.gcount());

    std::stringstream ss;
    ss << crc.checksum();
    return ss.str();
}
