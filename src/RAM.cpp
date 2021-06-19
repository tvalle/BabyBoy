#include "RAM.h"
#include <fstream>
#include <iterator>
#include <cstring>

const int tiles_x = 32;
const int tiles_y = 32;

const int width = 8 * tiles_x;
const int height = 8 * tiles_y;

RAM::RAM()
{
    //TODO: fill with random numbers in the same manner as gameboy does
    ram = std::vector<uint8_t>(0x10000);

    // TODO: remove, for testing only
    /*std::ifstream dumpFile("vram.dump", std::ios_base::binary);

    int pos = 0x8000;
    while (dumpFile) {
        ram[pos] = dumpFile.get();
        pos++;
    }*/

    matrix = new uint8_t * [height];
    for (int y = 0; y < height; y++)
    {
        matrix[y] = new uint8_t[width];
    }
}

void RAM::write8(uint16_t address, uint8_t value)
{
    if (address < 0x8000 || (address >= 0xFEA0 && address <= 0xFEFF))
        return;

    //echo
    if (address >= 0xE000 && address < 0xFE00)
    {
        ram[address - 0x2000] = value;
    }

    ram[address] = value;
}

uint8_t RAM::read(uint16_t address)
{
    return ram[address];
}

void RAM::copy(std::vector<uint8_t> src, std::size_t count)
{
    std::copy(src.begin(), src.begin() + count, ram.begin());
}

uint8_t** RAM::getVRAM_Tiles()
{
    const int tiles_x = 16;
    const int tiles_y = 24;

    const int width = 8 * tiles_x;
    const int height = 8 * tiles_y;

    uint8_t** matrix = new uint8_t * [height];
    for (int y = 0; y < height; y++)
    {
        matrix[y] = new uint8_t[width];
    }

    for (int tiles = 0; tiles <= 0x17F; tiles++)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                //int addr = (0x8000 + (tiles * 0x10)) + (i * 2);

                uint8_t pixel = (ram[(0x8000 + (tiles * 0x10)) + (i * 2)] >> (7 - j)) & 1;
                pixel = pixel << 1;
                pixel = pixel | ((ram[(0x8001 + (tiles * 0x10)) + (i * 2)] >> (7 - j)) & 1);

                matrix[i + ((tiles / 16) * 8)][j + ((tiles % 16) * 8)] = pixel;
            }
        }
    }

    //auto color = *((matrix + i * width) + j);
    return matrix;
}

uint8_t** RAM::getBGTileMapMatrix()
{
    uint16_t baseAddress = getLCDC_BGTileMap() ? 0x9c00 : 0x9800;
    
    for (int tiles = 0; tiles < 1024; tiles++)
    {
        uint8_t currentTile = ram[baseAddress + tiles];

        uint16_t tileAddress;

        if (getLCDC_BGTWindowTile())
        {
            // 8000-8fff - unsigned
            tileAddress = 0x8000 + (currentTile * 0x10);
        }
        else
        {
            // 8800-97ff - signed
            if (currentTile <= 127)
            {
                tileAddress = 0x9000 + (currentTile * 0x10);
            }
            else
            {
                tileAddress = 0x8800 + (currentTile * 0x10);
            }
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int x = 0;
                int y = 0;

                uint8_t pixel = (ram[tileAddress + (i * 2)] >> (7 - j)) & 1;
                pixel = pixel << 1;
                pixel = pixel | ((ram[tileAddress + 1 + (i * 2)] >> (7 - j)) & 1);

                matrix[i + ((tiles / tiles_x) * 8)][j + ((tiles % tiles_x) * 8)] = getPaletteColor(pixel);
            }
        }
    }

    //auto color = *((matrix + i * width) + j);
    return matrix;
}

bool RAM::getLCDC_BGTileMap()
{
    return (ram[0xFF40] & 0b00001000) == 0b00001000;
}

bool RAM::getLCDC_BGTWindowTile()
{
    return (ram[0xFF40] & 0b00010000) == 0b00010000;
}

bool RAM::getLCDC_IsDisplayOn()
{
    return (ram[0xFF40] & 0b10000000) == 0b10000000;
}

uint8_t RAM::getSCX()
{
    return ram[0xFF43];
}

uint8_t RAM::getSCY()
{
    return ram[0xFF42];
}

void RAM::writeToFF44(uint8_t value)
{
    ram[0xFF44] = value;
}

uint8_t RAM::getPaletteColor(uint8_t index)
{
    // FF47 is the palette "register"

    uint8_t palette[4] = { 0 };

    palette[0] = ram[0xFF47] & 0b00000011;
    palette[1] = (ram[0xFF47] & 0b0000110) >> 2;
    palette[2] = (ram[0xFF47] & 0b00110000) >> 4;
    palette[3] = (ram[0xFF47] & 0b11000000) >> 6;

    return palette[index];
}
