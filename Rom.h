#pragma once
#include<string>
#include<stdint.h>

class Rom
{
public:
    Rom(std::string fileName);

    char* romBuffer;
};

