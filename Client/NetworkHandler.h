#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <thread>
#include <chrono>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>

#include "constants.h"
#include "EncryptionHandler.h"
#include "FilesHandler.h"
#include "RequestsGenerator.h"
#include "StringHandler.h"



class NetworkHandler {
public:
    NetworkHandler(const std::string& client_name);

    bool serverResponse(boost::asio::ip::tcp::socket& socket, std::vector<std::string>& response_data);
    bool sendRegOrReconnectRequest(boost::asio::ip::tcp::socket& socket, const std::string& client_name, const std::string& client_uuid);
    bool sendPublicKeyRequest(boost::asio::ip::tcp::socket& socket, EncryptionHandler& encryption_handler,  const std::string& client_uuid);
    //bool sendFileRequest(boost::asio::ip::tcp::socket& socket, const std::string& file_path, EncryptionHandler& encryption_handler, const std::string& aes_key, const std::vector<std::string>& response_data);
    bool sendFileRequest(boost::asio::ip::tcp::socket& socket, const std::string& file_path, EncryptionHandler& encryption_handler, const std::string& aes_key, const std::vector<std::string>& response_data);
    bool handleFileTransfer(boost::asio::ip::tcp::socket& socket,EncryptionHandler& encryption_handler, FilesHandler& files_handler, std::string& file_path, std::string& aes_key, std::vector<std::string>& response_data, size_t& file_transfer_attempts);
    
    bool sendRequest(boost::asio::ip::tcp::socket& socket, uint16_t request_code);



private:
    RequestsGenerator requests_generator;
    StringHandler string_handler;
};
