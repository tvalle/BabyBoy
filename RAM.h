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

    uint8_t** getVRAM_Tiles();

    uint8_t** getBGTileMapMatrix();

    bool getLCDC_BGTileMap();
    bool getLCDC_BGTWindowTile();
private:

    uint8_t getPaletteColor(uint8_t index);

    std::array<uint8_t, 65535> ram;
};

