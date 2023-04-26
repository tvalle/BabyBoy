#include "RAM.h"
#include "Debug.h"
#include "Input.h"
#include <fstream>
#include <iterator>
#include <cstring>

const int tiles_x = 32;
const int tiles_y = 32;

const int width = 160;
const int height = 144;

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

    frameBuffer = new uint8_t * [height];
    for (int y = 0; y < height; y++)
    {
        frameBuffer[y] = new uint8_t[width];
    }
}

void RAM::write8(uint16_t address, uint8_t value)
{
    for (auto const& i : Debug::GetInstance()->addrWatchlist) {
        if (i == address) {
            Debug::GetInstance()->isPaused = true;
        }
    }

    if (address < 0x8000 || (address >= 0xFEA0 && address <= 0xFEFF))
        return;

    //echo
    if (address >= 0xE000 && address < 0xFE00)
    {
        ram[address - 0x2000] = value;
    }

    if (address == 0xFF00) {
        Input::GetInstance()->isAction = value & 0x20;
        Input::GetInstance()->isDirection = value & 0x10;
        return;
    }

    ram[address] = value;
}

uint8_t RAM::read(uint16_t address)
{
    // TODO: remove this later! This is for debugging with Gameboy Doctor only
    // if (address == 0xFF44)
    //     return 0x90;

    if (address == 0xFF00) {
        return Input::GetInstance()->getState();
    }

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
    // uint16_t baseAddress = getLCDC_BGTileMap() ? 0x9c00 : 0x9800;
    
    // for (int tiles = 0; tiles < 1024; tiles++)
    // {
    //     uint8_t currentTile = ram[baseAddress + tiles];

    //     uint16_t tileAddress;

    //     if (getLCDC_BGWindowTileData())
    //     {
    //         // 8000-8fff - unsigned
    //         tileAddress = 0x8000 + (currentTile * 0x10);
    //     }
    //     else
    //     {
    //         // 8800-97ff - signed
    //         if (currentTile <= 127)
    //         {
    //             tileAddress = 0x9000 + (currentTile * 0x10);
    //         }
    //         else
    //         {
    //             tileAddress = 0x8800 + (currentTile * 0x10);
    //         }
    //     }

    //     for (int i = 0; i < 8; i++)
    //     {
    //         for (int j = 0; j < 8; j++)
    //         {
    //             int x = 0;
    //             int y = 0;

    //             uint8_t pixel = (ram[tileAddress + (i * 2)] >> (7 - j)) & 1;
    //             pixel = pixel << 1;
    //             pixel = pixel | ((ram[tileAddress + 1 + (i * 2)] >> (7 - j)) & 1);

    //             frameBuffer[i + ((tiles / tiles_x) * 8)][j + ((tiles % tiles_x) * 8)] = getPaletteColor(pixel);
    //         }
    //     }
    // }

    //auto color = *((matrix + i * width) + j);
    return frameBuffer;
}

bool RAM::getLCDC_BGTileMap()
{
    return (ram[0xFF40] & 0b00001000) == 0b00001000;
}

bool RAM::getLCDC_BGWindowTileData()
{
    return (ram[0xFF40] & 0b00010000) == 0b00010000;
}

bool RAM::getLCDC_IsDisplayOn()
{
    return (ram[0xFF40] & 0b10000000) == 0b10000000;
}

bool RAM::hasIE()
{
    return ram[0xFFFF];
}

bool RAM::hasIF()
{
    return ram[0xFF0F];
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
    // FF47 is the palette register

    uint8_t palette[4] = { 0 };

    palette[0] = ram[0xFF47]  & 0b00000011;
    palette[1] = (ram[0xFF47] & 0b00001100) >> 2;
    palette[2] = (ram[0xFF47] & 0b00110000) >> 4;
    palette[3] = (ram[0xFF47] & 0b11000000) >> 6;

    return palette[index];
}

int RAM::getTileForX(int x)
{
    const int TILE_SIZE = 8;
    const int HORIZONTAL_TILES = 20;
    const int VERTICAL_TILES = 18;

    auto scx = getSCX() / TILE_SIZE;

    auto scy = getSCY() / TILE_SIZE;
    auto y = ram[0xFF44] / TILE_SIZE; // 0xFF44 = current scanline

    uint16_t baseAddress = getLCDC_BGTileMap() ? 0x9c00 : 0x9800;

    y = ((y + scy) % 32) * 32;
    x = (x + scx) % 32;

    return ram[baseAddress + y + x];
}

uint16_t RAM::getTileAddress(int tileId)
{
    uint16_t tileAddress;

    if (getLCDC_BGWindowTileData())
    {
        // 8000-8fff - unsigned
        tileAddress = 0x8000 + (tileId * 0x10);
    }
    else
    {
        // 8800-97ff - signed
        if (tileId <= 127)
        {
            tileAddress = 0x9000 + (tileId * 0x10);
        }
        else
        {
            tileAddress = 0x8800 + (tileId * 0x10);
        }
    }

    return tileAddress;
}

void RAM::fillFrameBufferWithTile(uint16_t tileAddress, int x)
{
    // 32768

    auto y = ram[0xFF44] % 8;

    for (int j = 0; j < 8; j++)
    {
        uint8_t pixel = (ram[tileAddress + (y * 2)] >> (7 - j)) & 1;
        pixel = pixel << 1;
        pixel = pixel | ((ram[tileAddress + 1 + (y * 2)] >> (7 - j)) & 1);

        auto offsetY = ram[0xFF44] - (getSCY() % 8);

        if (offsetY >= 0)
        {
            frameBuffer[offsetY][(x * 8) + j] = getPaletteColor(pixel);
        }
    }
}

void RAM::setLCDMode(uint8_t mode)
{
    ram[0xFF41] = (ram[0xFF41] & 0b11111100) | mode;
}

void RAM::setLYCeqLY()
{
    bool isEqual = ram[0xFF44] == ram[0xFF45];

    ram[0xFF41] = (ram[0xFF41] & 0b11111011) | (isEqual ? 0x4: 0x0);
}

uint8_t RAM::getLCDMode()
{
    return ram[0xFF41] & 0b00000011;
}
