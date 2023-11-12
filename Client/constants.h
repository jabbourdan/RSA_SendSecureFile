#pragma once

#include <string>

namespace REQUEST {
	// REQUEST CODES
	const uint16_t REGISTRATION = 1025;
	const uint16_t KEYS_EXCHANGE = 1026;
	const uint16_t RECONNECT = 1027;
	const uint16_t FILE_TRANSFER = 1028;
	const uint16_t VALID_CKSUM_FINISH = 1029;
	const uint16_t INVALID_CKSUM_RESEND = 1030;
	const uint16_t INVALID_CKSUM_FINISH = 1031;
}

namespace MAX_BYTES {	// MAX SIZE IN BYTES
	const size_t REQUEST_CODE = 2;
	const size_t CLIENT_ID = 16;
	const size_t CLIENT_NAME = 255;
	const size_t PUBLIC_KEY = 160;
	const size_t VERSION = 1;
	const size_t PAYLOAD_SIZE = 4;
	const size_t FILE_SIZE = 4;
	const size_t FILE_NAME = 255;
}

namespace RESPONSE {
	const uint16_t REGISTRATION_ACCEPT = 2100;
	const uint16_t REGISTRATION_FAIL = 2101;
	const uint16_t RSA_KEY_ACCEPT = 2102;
	const uint16_t FILE_TRANSFER_SUCCESS = 2103;
	const uint16_t CRC_FINISH = 2104;
	const uint16_t RECONNECT_ACCEPT = 2105;
	const uint16_t RECONNECT_FAIL = 2106;
	const uint16_t INVALID_REQUEST = 2107;
}

namespace FILES {
	const std::string TRANSFER_FILE = "transfer.info";
	const std::string ME_FILE = "me.info";
}

namespace NETWORK {
	const std::string DEFAULT_IP = "127.0.0.1";
	const std::string DEFAULT_PORT = "1357";
	const size_t MIN_PORT_NUMBER = 0;
	const size_t MAX_PORT_NUMBER = 65535;
	const size_t MAX_REQUEST_RETRIES = 3;
}

namespace CLIENT {
	const uint8_t VERSION = 1;
}

