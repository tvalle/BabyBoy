#include "RAM.h"

RAM::RAM()
{
    //TODO: fill with random numbers in the same manner as gameboy does
    ram.fill(0);
}

void RAM::write8(uint16_t address, uint8_t value)
{
    ram[address] = value;
}

uint8_t RAM::read(uint16_t address)
{
    return ram[address];
}
