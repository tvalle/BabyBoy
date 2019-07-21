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
    case 0x03:
        // inc bc
        c++;
        if (c == 0)
            b++;

        PC++;
        break;
    case 0x13:
        // inc de
        e++;
        if (e == 0)
            d++;

        PC++;
        break;
    case 0x23:
        // inc hl
        l++;
        if (l == 0)
            h++;

        PC++;
        break;
    case 0x33:
        // inc sp
        SP++;
        PC++;
        break;
    case 0x04:
        //inc b
        increaseRegister(&b);
        PC++;
        break;
    case 0x05:
        //dec b
        decreaseRegister(&b);
        PC++;
        break;
    case 0x0C:
        //inc C
        increaseRegister(&c);
        PC++;
        break;
    case 0x0D:
        //dec C
        decreaseRegister(&c);
        PC++;
        break;
    case 0x14:
        //inc D
        increaseRegister(&d);
        PC++;
        break;
    case 0x15:
        //dec D
        decreaseRegister(&d);
        PC++;
        break;
    case 0x24:
        //inc H
        increaseRegister(&h);
        PC++;
        break;
    case 0x25:
        //dec H
        decreaseRegister(&h);
        PC++;
        break;
    case 0x1C:
        //inc E
        increaseRegister(&e);
        PC++;
        break;
    case 0x1D:
        //dec E
        decreaseRegister(&e);
        PC++;
        break;
    case 0x2C:
        //inc L
        increaseRegister(&l);
        PC++;
        break;
    case 0x2D:
        //dec L
        decreaseRegister(&l);
        PC++;
        break;
    case 0x3C:
        //inc A
        increaseRegister(&a);
        PC++;
        break;
    case 0x3D:
        //dec A
        decreaseRegister(&a);
        PC++;
        break;

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

void CPU::setZ(bool value)
{
}

void CPU::setN(bool value)
{
}

void CPU::setH(bool value)
{
}

void CPU::setC(bool value)
{
}

uint16_t CPU::combineRegisters(uint8_t reg1, uint8_t reg2)
{
    return (reg1 << 8 | 0x00ff) & (reg2 | 0xff00);
}

void CPU::increaseRegister(uint8_t* reg)
{
    (*reg)++;
    if (*reg == 0)
    {
        setZ(true);
        setH(true);
    }
    else
    {
        setZ(false);
        setH(false);
    }
}

void CPU::decreaseRegister(uint8_t* reg)
{
    (*reg)--;

    if (*reg == 0)
        setZ(true);
    else
        setZ(false);

    if (*reg == 0xFF)
        setH(true);
    else
        setH(false);

    setN(true);
}
