#pragma once
#include<string>
#include<stdint.h>

class Rom
{
public:
    Rom(std::string fileName);

    uint8_t fetchByte(int position);
private:
    char* romBuffer;
};

