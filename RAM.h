#pragma once
#include <stdint.h>
#include <array>
class RAM
{
public: 
    RAM();

    void write8(uint16_t address, uint8_t value);
    uint8_t read(uint16_t address);

    void* memcpy(const void* src, std::size_t count);

private:

    std::array<uint8_t, 65535> ram;
};

