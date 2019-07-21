#include "CPU.h"

CPU::CPU(Rom rom) : rom("")
{
    PC = 0x100;

    a = b = c = d = e = f = h = l = 0;

    this->rom = rom;
}

void CPU::ExecuteInstruction(uint8_t instruction)
{
    printf("%x ", instruction);

    switch (instruction)
    {
    case 0x00:
        PC++;
        break;
    case 0xAF:
        a = a ^ a;
        PC++;
        break;

        // INCREASE DECREASE OPERATIONS

        // JUMP OPERATIONS
    case 0xC2:
        // jp nz, **
        PC = !isFlagSet(Flag::Z) ? fetchNext2BytesInverted(rom, PC) : PC += 3;
        break;
    case 0xC3:
        // jp **
        PC = fetchNext2BytesInverted(rom, PC);
        break;
    case 0xCA:
        // jp z, **
        PC = isFlagSet(Flag::Z) ? fetchNext2BytesInverted(rom, PC) : PC += 3;
        break;
    case 0xD2:
        // jp nc, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(rom, PC) : PC += 3;
        break;
    case 0xDA:
        // jp c, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(rom, PC) : PC += 3;
        break;
    case 0xE9:
        // jp (hl), **
        PC = combineRegisters(h, l);
        break;
    default:
        break;
    }
}

uint16_t CPU::fetchNext2BytesInverted(Rom rom, int PC)
{
    return (rom.fetchByte(PC + 1) | 0xff00) & ((rom.fetchByte(PC + 2) << 8) | 0x00ff);
}

bool CPU::isFlagSet(Flag flag)
{
    return f & (1 << flag);
}

uint16_t CPU::combineRegisters(uint8_t reg1, uint8_t reg2)
{
    return (reg1 << 8 | 0x00ff) & (reg2 | 0xff00);
}
