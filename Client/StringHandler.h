#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "constants.h"
#include "RequestsGenerator.h"


class StringHandler
{
public:
	std::string serializeRequest(const Request& request);
	int stringToInt(const std::string& str);
	std::vector<std::string> splitString(std::string str, char delimiter);

};

