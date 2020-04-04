#pragma once
#include <stdint.h>
class RAM
{
public: 
    void write8(uint16_t address, uint8_t value);
    uint8_t read(uint16_t address);
};

