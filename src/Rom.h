#pragma once
#include <vector>
#include <string>
#include <stdint.h>

class Rom
{
public:
	Rom();
	Rom(std::string fileName);

	std::vector<uint8_t> romBuffer;
};
