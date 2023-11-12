#pragma once

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include "constants.h"

struct Request {

    // Header
    std::string client_uuid;
    size_t version = 0;
    uint16_t request_code = 0;
    uint32_t payload_size = 0;

    // Payload
    std::string client_name;
    std::string public_key;
    uint32_t file_size = 0;
    std::string file_name;
    std::string file_content;
};

class RequestsGenerator
{
public:

    // Public functions
    RequestsGenerator(const std::string& client_name);

    void generateRequest(Request& request, size_t request_code);

    // Setters
    void setClientID(const std::string &client_id);
    void setClientName(const std::string &client_name);
    void set_public_key(const std::string& public_key);
    void setFileSize(uint32_t file_size);
    void setFileName(const std::string& file_name);
    void setFileContent(const std::string& file_content);

private:

    // Private variables
    std::string client_uuid;
    std::string client_name;
    std::string public_key;
    uint32_t file_size;
    std::string file_name;
    std::string file_content;

    // Private funcitons
    RequestsGenerator();

    void getRegRequestPayload(Request &request);
    void getKeyRequestPayload(Request& request);
    void getReconnectPayload(Request& request);
    void getFileRequestPayload(Request& request);
    void getValidCksumrequestPayload(Request& request);
    void getInvalidCksumPayload(Request& request);
    void getInvalidCksumRequestPayload(Request& request);

};
