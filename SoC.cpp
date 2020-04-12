#include "SoC.h"

SoC::SoC(Rom rom)
{
    ram = RAM();
    cpu = CPU(ram);

    if (hasBoot)
    {
        Rom bootRom = Rom("dmg0_rom.bin");

        auto romSize = std::clamp(static_cast<int>(rom.romBuffer.size()), 0, 0x7FFF);
        auto bootRomSize = std::clamp(static_cast<int>(bootRom.romBuffer.size()), 0, 0x7FFF);
        ram.memcpy(&rom.romBuffer[0], romSize);
        ram.memcpy(&bootRom.romBuffer[0], bootRomSize);
    }

    isRunning = true;
}

void SoC::step()
{
    cpu.ExecuteInstruction(ram.read(cpu.PC));
}
