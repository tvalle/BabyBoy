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

    bool isPaused;

    void step();

    RAM ram;
    CPU cpu;

private:
    int modeclock;
    int graphicsMode;

    bool hasBIOSbeenDisabled = false;
    std::vector<uint8_t> backupInitRom; // ROM from 0x00 to 0xFF to read back when 0xFF50 is set

    uint8_t currentScanline();
    void increaseScanline();

    void gpuStep();
    void drawCurrentLine();

    void setInitialValuesWhenNoBoot();
};
