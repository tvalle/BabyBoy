#pragma once
#include "Rom.h"
#include "RAM.h"
#include "CPU.h"

class SoC
{
public:
    SoC(Rom rom);

    bool isRunning;

    void step();

private:

    RAM ram;
    CPU cpu;

    bool hasBoot = true;
};

