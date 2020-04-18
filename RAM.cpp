#include "RAM.h"
#include <fstream>
#include <iterator>

RAM::RAM()
{
    //TODO: fill with random numbers in the same manner as gameboy does
    ram.fill(0);

    // TODO: remove, for testing only
    std::ifstream dumpFile("vram.dump", std::ios_base::binary);

    int pos = 0x8000;
    while (dumpFile) {
        ram[pos] = dumpFile.get();
        pos++;
    }

}

void RAM::write8(uint16_t address, uint8_t value)
{
    ram[address] = value;
}

uint8_t RAM::read(uint16_t address)
{
    return ram[address];
}

void* RAM::memcpy(const void* src, std::size_t count)
{
    return std::memcpy(&ram, src, count);
}

void RAM::getVRAM_Tiles(uint8_t* matrix)
{
    const uint8_t tiles_x = 16;
    const uint8_t tiles_y = 16;

    const uint8_t width = 8 * tiles_x;
    const uint8_t height = 8 * tiles_y;

    //uint8_t matrix[height][width];

    for (int tiles = 0; tiles < 0xFF; tiles++)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                uint8_t pixel = (ram[(0x8000 + (tiles * 0x10)) + (i * 2)] >> (7 - j)) & 1;
                pixel = pixel << 1;
                pixel = pixel | ((ram[(0x8001 + (tiles * 0x10)) + (i * 2)] >> (7 - j)) & 1);

                //matrix[i + ((tiles / 16) * 8)][j + ((tiles % 16) * 8)] = pixel;
            }
        }
    }

    //auto color = *((matrix + i * width) + j);
}
