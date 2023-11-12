#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <regex>
#include <string>
#include <vector>

#include <memory>
#include <iomanip>
#include <cstdint>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>


#include "constants.h"

class FilesHandler {
public:
    FilesHandler() = default;
    ~FilesHandler() = default;

    bool initVariables(std::string& client_name, std::string& client_uuid, std::string& private_key, std::string& file_path, std::string& file_name);
    bool fileExist(const std::string& filename);
    std::string readClient();
    std::string readClientID();
    std::string readPrivateKey();
    std::string readIPAddress();
    std::string readPort();
    std::string readFilePath();
    std::string calculateCRC(const std::string& file_path);
    std::string extractFileNameFromTransferInfo();
    void createME(std::string client_name, std::string client_uuid, std::string private_key);


};
