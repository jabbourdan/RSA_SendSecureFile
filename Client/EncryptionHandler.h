#pragma once

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/ccm.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/oaep.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>


class EncryptionHandler
{
public:
    EncryptionHandler();
    std::string getPrivateKey() const;
    std::string getPublicKey() const;
    CryptoPP::SecByteBlock decodeBase64Key(const std::string& aes_key_base64);
    std::string encryptFile(const std::string& filename, const CryptoPP::SecByteBlock& key);
    std::string decryptAESKeyPrivateRSAKey(const std::string& encrypted_aes_key, const std::string& private_RSA);
    std::string encryptFileBinary(const std::string& file_path, const CryptoPP::SecByteBlock& aes_key);
private:
    // private variables:
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;
    std::string AES_key;

    // private funcitons:
    void printHex(const std::string& content);
    std::string privateKeyToString(const CryptoPP::RSA::PrivateKey& private_key);
    CryptoPP::RSA::PrivateKey stringToPrivateKey(const std::string& private_key_str);

};
