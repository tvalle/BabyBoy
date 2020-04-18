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

    RAM ram;
    CPU cpu;

private:

    bool hasBoot = true;
};

