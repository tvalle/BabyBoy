#include "SoC.h"
#include "Config.h"

SoC::SoC(Rom rom)
{
    ram = RAM();
    cpu = CPU(ram);

    if (Config::GetInstance()->HasBoot())
    {
        Rom bootRom = Rom(Config::GetInstance()->GetBootFileName());

        auto romSize = std::clamp(static_cast<int>(rom.romBuffer.size()), 0, 0x7FFF);
        auto bootRomSize = std::clamp(static_cast<int>(bootRom.romBuffer.size()), 0, 0x7FFF);
        ram.copy(rom.romBuffer, romSize);
        ram.copy(bootRom.romBuffer, bootRomSize);
    }

    modeclock = 0;
    graphicsMode = 2;

    isRunning = true;
}

void SoC::step()
{
    cpu.ExecuteInstruction(ram.read(cpu.PC));
    cpu.cycles += cpu.lastInstructionCycle;
    gpuStep();
}

void SoC::gpuStep()
{
    modeclock += cpu.lastInstructionCycle;

    switch (graphicsMode)
    {
        // OAM read mode, scanline active
    case 2:
        if (modeclock >= 80)
        {
            // Enter scanline mode 3
            modeclock = 0;
            graphicsMode = 3;
        }
        break;

        // VRAM read mode, scanline active
        // Treat end of mode 3 as end of scanline
    case 3:
        if (modeclock >= 172)
        {
            // Enter hblank
            modeclock = 0;
            graphicsMode = 0;

            // Write a scanline to the framebuffer
            //GPU.renderscan();
        }
        break;

        // Hblank
        // After the last hblank, push the screen data to canvas
    case 0:
        if (modeclock >= 204)
        {
            modeclock = 0;
            increaseScanline();

            if (currentScanline() == 143)
            {
                // Enter vblank
                graphicsMode = 1;
                //GPU._canvas.putImageData(GPU._scrn, 0, 0);
            }
            else
            {
                graphicsMode = 2;
            }
        }
        break;

        // Vblank (10 lines)
    case 1:
        if (modeclock >= 456)
        {
            modeclock = 0;
            increaseScanline();

            if (currentScanline() > 153)
            {
                // Restart scanning modes
                graphicsMode = 2;
                ram.writeToFF44(0);
            }
        }
        break;
    }
}

uint8_t SoC::currentScanline()
{
    return ram.read(0xFF44);
}

void SoC::increaseScanline()
{
    auto scanline = currentScanline();
    ram.writeToFF44(++scanline);
}
