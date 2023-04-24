#pragma once
#include "Rom.h"
#include "RAM.h"
#include "CPU.h"
#include <algorithm>

class SoC
{
public:
    SoC();
    SoC(Rom rom);

    void step(uint8_t keysPressed);

    RAM ram;
    CPU cpu;

    int getGraphicsMode();

private:
    int modeclock;
    uint8_t graphicsMode;

    bool hasBIOSbeenDisabled = false;
    std::vector<uint8_t> backupInitRom; // ROM from 0x00 to 0xFF to read back when 0xFF50 is set

    uint8_t currentScanline();
    void increaseScanline();

    void gpuStep();
    void drawCurrentLine();

    void setInitialValuesWhenNoBoot();
};
