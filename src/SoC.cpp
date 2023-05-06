#include "SoC.h"
#include "Config.h"
#include "Debug.h"

SoC::SoC()
{
}

SoC::SoC(Rom rom)
{
    ram = RAM();
    cpu = CPU(ram);

    if (Config::GetInstance()->HasBoot())
    {
        Rom bootRom = Rom(Config::GetInstance()->GetBootFileName());

        backupInitRom = std::vector<uint8_t>(0xFF);
        std::copy(rom.romBuffer.begin(), rom.romBuffer.begin() + 0xFF, backupInitRom.begin());

        auto romSize = std::clamp(static_cast<int>(rom.romBuffer.size()), 0, 0x7FFF);
        auto bootRomSize = std::clamp(static_cast<int>(bootRom.romBuffer.size()), 0, 0x7FFF);
        ram.copy(rom.romBuffer, romSize);
        ram.copy(bootRom.romBuffer, bootRomSize);
    }
    else
    {
        auto romSize = std::clamp(static_cast<int>(rom.romBuffer.size()), 0, 0x7FFF);
        ram.copy(rom.romBuffer, romSize);
        setInitialValuesWhenNoBoot();
    }

    modeclock = 0;
    graphicsMode = 2;

    for (int i = 0; i < BREAKPOINT_SIZE; i++) {
        Debug::GetInstance()->breakpoints[i] = false;
    }

    // Debug::GetInstance()->breakpoints[0x101] = true;
    // Debug::GetInstance()->addrWatchlist.insert(Debug::GetInstance()->addrWatchlist.begin(), 0xdd02);
}

void SoC::step()
{
    cpu.ExecuteInstruction(ram.read(cpu.PC));
    if (Debug::GetInstance()->breakpoints[cpu.PC] == true) {
        Debug::GetInstance()->isPaused = true;
    }

    //Check if Boot was disabled
    if (ram.read(0xFF50) != 0 && !hasBIOSbeenDisabled)
    {
        hasBIOSbeenDisabled = true;
        ram.copy(backupInitRom, 0xFF);
    }

    if (cpu.IME && ram.read(0xFF0F) && ram.read(0xFFFF))
    {
        uint8_t interrupts = ram.read(0xFF0F) & ram.read(0xFFFF);
        uint8_t if_flag = ram.read(0xFF0F);

        // VBlank interrupt
        if (interrupts & 0b00001)
        {
            ram.write8(0xFF0F, if_flag & (~0b00001));
            cpu.handleInterrupt(0x40);
        } 
        // LCD STAT interrupt
        else if (interrupts & 0b00010)
        {
            ram.write8(0xFF0F, if_flag & (~0b00010));
            cpu.handleInterrupt(0x48);
        }
        // Timer interrupt
        else if (interrupts & 0b00100)
        {
            ram.write8(0xFF0F, if_flag & (~0b00100));
            cpu.handleInterrupt(0x50);
        }
        // Serial interrupt
        else if (interrupts & 0b01000)
        {
            ram.write8(0xFF0F, if_flag & (~0b01000));
            cpu.handleInterrupt(0x58);
        }
        // Joypad interrupt
        else if (interrupts & 0b10000)
        {
            ram.write8(0xFF0F, if_flag & (~0b10000));
            cpu.handleInterrupt(0x60);
        }

        if (Debug::GetInstance()->breakpoints[cpu.PC] == true) {
            Debug::GetInstance()->isPaused = true;
        }
    }

    cpu.cycles += cpu.lastClockCycle;
    gpuStep();
}

void SoC::gpuStep()
{
    if (!ram.getLCDC_IsDisplayOn())
        return;

    modeclock += cpu.lastClockCycle;

    switch (graphicsMode)
    {
        // OAM read mode, scanline active
    case 2:
        if (modeclock >= 80)
        {
            // Enter scanline mode 3
            modeclock = 0;
            graphicsMode = 3;
            ram.setLCDMode(graphicsMode);
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
            ram.setLCDMode(graphicsMode);

            // Write a scanline to the framebuffer
            drawCurrentLine();
        }
        break;

        // Hblank
        // After the last hblank, push the screen data to canvas
    case 0:
        if (modeclock >= 204)
        {
            modeclock = 0;
            increaseScanline();
            // ram.setLYCeqLY();

            if (currentScanline() == 143)
            {
                // Enter vblank
                graphicsMode = 1;
                ram.setLCDMode(graphicsMode);
                //GPU._canvas.putImageData(GPU._scrn, 0, 0);
                auto vblank_interrupt = ram.read(0xFF0F);
                vblank_interrupt |= 1;
                ram.write8(0xFF0F, vblank_interrupt);
            }
            else
            {
                graphicsMode = 2;
                ram.setLCDMode(graphicsMode);
            }
        }
        break;

        // Vblank (10 lines)
    case 1:
        if (modeclock >= 456)
        {
            modeclock = 0;
            increaseScanline();

            if (currentScanline() >= 153)
            {
                // Restart scanning modes
                graphicsMode = 2;
                ram.setLCDMode(graphicsMode);
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

void SoC::setInitialValuesWhenNoBoot()
{
    cpu.PC = 0x100;

    cpu.a = 0x01;
    cpu.f = 0xB0;
    cpu.b = 0x00;
    cpu.c = 0x13;
    cpu.d = 0x00;
    cpu.e = 0xD8;
    cpu.h = 0x01;
    cpu.l = 0x4D;

    cpu.SP = 0xFFFE;

    ram.write8(0xFF05, 0x00);
    ram.write8(0xFF06, 0x00);
    ram.write8(0xFF07, 0x00);
    ram.write8(0xFF10, 0x80);
    ram.write8(0xFF11, 0xBF);
    ram.write8(0xFF12, 0xF3);
    ram.write8(0xFF14, 0xBF);
    ram.write8(0xFF16, 0x3F);
    ram.write8(0xFF17, 0x00);
    ram.write8(0xFF19, 0xBF);
    ram.write8(0xFF1A, 0x7F);
    ram.write8(0xFF1B, 0xFF);
    ram.write8(0xFF1C, 0x9F);
    ram.write8(0xFF1E, 0xBF);
    ram.write8(0xFF20, 0xFF);
    ram.write8(0xFF21, 0x00);
    ram.write8(0xFF22, 0x00);
    ram.write8(0xFF23, 0xBF);
    ram.write8(0xFF24, 0x77);
    ram.write8(0xFF25, 0xF3);
    ram.write8(0xFF26, 0xF1);
    ram.write8(0xFF40, 0x91);
    ram.write8(0xFF42, 0x00);
    ram.write8(0xFF43, 0x00);
    ram.write8(0xFF45, 0x00);
    ram.write8(0xFF47, 0xFC);
    ram.write8(0xFF48, 0xFF);
    ram.write8(0xFF49, 0xFF);
    ram.write8(0xFF4A, 0x00);
    ram.write8(0xFF4B, 0x00);
    ram.write8(0xFFFF, 0x00);
}

void SoC::drawCurrentLine()
{
    auto y = currentScanline();

    if (y >= 144)
        return;

    // 20 = 160 / 8 (Number of horizontal tiles)
    for (int i = 0; i < 20; i++)
    {
        // Draw BG
        auto tileId = ram.getTileForX(i);
        auto tileAddress = ram.getTileAddress(tileId);
        ram.fillFrameBufferWithTile(tileAddress, i);

        // Draw Sprite

        // Draw Window
    }
}

int SoC::getGraphicsMode() {
    return ram.read(0xFF41) & 0b00000011;
}