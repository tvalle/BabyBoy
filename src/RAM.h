#pragma once
#include <stdint.h>
#include <vector>
class RAM
{
public: 
    RAM();

    void write8(uint16_t address, uint8_t value);
    uint8_t read(uint16_t address);

    void copy(std::vector<uint8_t> origin, std::size_t count);

    uint8_t** getVRAM_Tiles();

    uint8_t** getBGTileMapMatrix();

    bool getLCDC_BGTileMap();
    bool getLCDC_BGWindowTileData();
    bool getLCDC_IsDisplayOn();

    uint8_t getSCX();
    uint8_t getSCY();

    void writeToFF44(uint8_t value);

    int getTileForX(int x);
    uint16_t getTileAddress(int tileId);
    void fillFrameBufferWithTile(uint16_t tileAddress, int x);

    void setLCDMode(uint8_t mode);
    uint8_t getLCDMode();

private:

    uint8_t getPaletteColor(uint8_t index);

    std::vector<uint8_t> ram;

    uint8_t** frameBuffer;
};

