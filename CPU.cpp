#include "CPU.h"

CPU::CPU(Rom rom)
{
    PC = 0x100;
    SP = 0;

    a = b = c = d = e = f = h = l = 0;

    std::memcpy(RAM, rom.romBuffer, 0x7FFF);
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

        // INCREASE DECREASE OPERATIONS *****************************************
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

        // LOGICAL OPERATIONS **************************************
        //TODO: ON COMPARE OPERATIONS CHECK HOW TO SET H AND C CORRECTLY. EX A=10 AND CP VALUE 90 WILL TRIGGER C BUT NOT H
    case 0xBE:
        // cp (hl)
        compare(RAM[combineRegisters(h, l)]);
        PC++;
    case 0xFE:
        // cp *
        compare(RAM[PC + 1]);
        PC += 2;
        break;

        // JUMP OPERATIONS *****************************************
    case 0X20:
        // jr nz, *
        if (!isFlagSet(Flag::Z))
            PC += RAM[PC + 1];

        PC += 2;
        break;
    case 0xC2:
        // jp nz, **
        PC = !isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xC3:
        // jp **
        PC = fetchNext2BytesInverted(PC);
        break;
    case 0xCA:
        // jp z, **
        PC = isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xD2:
        // jp nc, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xDA:
        // jp c, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xE9:
        // jp (hl), **
        PC = combineRegisters(h, l);
        break;

        // PUSH OPERATIONS *****************************************
    case 0xE5:
        // push hl
        RAM[SP--] = h;
        RAM[SP--] = l;
        PC++;
        break;

        // LOAD OPERATIONS *****************************************
    case 0x01:
        // ld bc, **
        c = RAM[PC + 1];
        b = RAM[PC + 2];
        PC += 3;
        break;
    case 0x11:
        // ld de, **
        e = RAM[PC + 1];
        d = RAM[PC + 2];
        PC += 3;
        break;
    case 0x21:
        // ld hl **
        l = RAM[PC + 1];
        h = RAM[PC + 2];
        PC += 3;
        break;
    case 0x31:
        // ld sp **
        SP = fetchNext2BytesInverted(PC);
        PC += 3;
        break;
    case 0x02:
        // ld (bc), a
        RAM[combineRegisters(b, c)] = a;
        PC += 1;
        break;
    case 0x12:
        // ld (de), a
        RAM[combineRegisters(d, e)] = a;
        PC += 1;
        break;
    case 0x22:
        // ldi (hl), a
        RAM[combineRegisters(h, l)] = a;
        increaseRegister(&h, &l);
        PC++;
        break;
    case 0x32:
        // ldd (hl), a
        RAM[combineRegisters(h, l)] = a;
        decreaseRegister(&h, &l);
        PC++;
        break;
    case 0x06:
        // ld b, *
        b = RAM[PC + 1];
        PC += 2;
        break;
    case 0x16:
        // ld d, *
        d = RAM[PC + 1];
        PC += 2;
        break;
    case 0x26:
        // ld h, *
        h = RAM[PC + 1];
        PC += 2;
        break;
    case 0x36:
        // ld h, *
        h = RAM[PC + 1];
        PC += 2;
        break;
    case 0x08:
        // ld (a16), SP
        //TODO: IMPLEMENT (will have to jump to two memory points and store masked SP for each byte in it
        PC += 3;
        break;
    case 0x0A:
        // ld a, (bc)
        a = RAM[combineRegisters(b, c)];
        PC++;
        break;
    case 0x1A:
        // ld a, (de)
        a = RAM[combineRegisters(d, e)];
        PC++;
        break;
    case 0x2A:
        // ldi a, (hl)
        a = RAM[combineRegisters(h, l)];
        increaseRegister(&h, &l);
        PC++;
        break;
    case 0x3A:
        // ldd a, (hl)
        a = RAM[combineRegisters(h, l)];
        decreaseRegister(&h, &l);
        PC++;
        break;
    case 0x0E:
        // ld c, *
        c = RAM[PC + 1];
        PC += 2;
        break;
    case 0x1E:
        // ld e, *
        e = RAM[PC + 1];
        PC += 2;
        break;
    case 0x2E:
        // ld l, *
        l = RAM[PC + 1];
        PC += 2;
        break;
    case 0x3E:
        // ld a, *
        a = RAM[PC + 1];
        PC += 2;
        break;
    case 0x40:
        // ld b, b
        b = b;
        PC++;
        break;
    case 0x41:
        // ld b, c
        b = c;
        PC++;
        break;
    case 0x42:
        // ld b, d
        b = d;
        PC++;
        break;
    case 0x43:
        // ld b, e
        b = e;
        PC++;
        break;
    case 0x44:
        // ld b, h
        b = h;
        PC++;
        break;
    case 0x45:
        // ld b, l
        b = l;
        PC++;
        break;
    case 0x46:
        // ld b, (hl)
        b = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x47:
        // ld b, a
        b = a;
        PC++;
        break;
    case 0x48:
        // ld c, b
        c = b;
        PC++;
        break;
    case 0x49:
        // ld c, c
        c = c;
        PC++;
        break;
    case 0x4A:
        // ld c, d
        c = d;
        PC++;
        break;
    case 0x4B:
        // ld c, e
        c = e;
        PC++;
        break;
    case 0x4C:
        // ld c, h
        c = h;
        PC++;
        break;
    case 0x4D:
        // ld c, l
        c = l;
        PC++;
        break;
    case 0x4E:
        // ld c, (hl)
        c = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x4F:
        // ld c, a
        c = a;
        PC++;
        break;
    case 0x50:
        // ld d, b
        d = b;
        PC++;
        break;
    case 0x51:
        // ld d, c
        d = c;
        PC++;
        break;
    case 0x52:
        // ld d, d
        d = d;
        PC++;
        break;
    case 0x53:
        // ld d, e
        d = e;
        PC++;
        break;
    case 0x54:
        // ld d, h
        d = h;
        PC++;
        break;
    case 0x55:
        // ld d, l
        d = l;
        PC++;
        break;
    case 0x56:
        // ld d, (hl)
        d = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x57:
        // ld d, a
        d = a;
        PC++;
        break;
    case 0x58:
        // ld e, b
        e = b;
        PC++;
        break;
    case 0x59:
        // ld e, c
        e = c;
        PC++;
        break;
    case 0x5A:
        // ld e, d
        e = d;
        PC++;
        break;
    case 0x5B:
        // ld e, e
        e = e;
        PC++;
        break;
    case 0x5C:
        // ld e, h
        e = h;
        PC++;
        break;
    case 0x5D:
        // ld e, l
        e = l;
        PC++;
        break;
    case 0x5E:
        // ld e, (hl)
        e = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x5F:
        // ld e, a
        e = a;
        PC++;
        break;
    case 0x60:
        // ld h, b
        h = b;
        PC++;
        break;
    case 0x61:
        // ld h, c
        h = c;
        PC++;
        break;
    case 0x62:
        // ld h, d
        h = d;
        PC++;
        break;
    case 0x63:
        // ld h, e
        h = e;
        PC++;
        break;
    case 0x64:
        // ld h, h
        h = h;
        PC++;
        break;
    case 0x65:
        // ld h, l
        h = l;
        PC++;
        break;
    case 0x66:
        // ld h, (hl)
        h = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x67:
        // ld h, a
        h = a;
        PC++;
        break;
    case 0x68:
        // ld l, b
        l = b;
        PC++;
        break;
    case 0x69:
        // ld l, c
        l = c;
        PC++;
        break;
    case 0x6A:
        // ld l, d
        l = d;
        PC++;
        break;
    case 0x6B:
        // ld l, e
        l = e;
        PC++;
        break;
    case 0x6C:
        // ld l, h
        l = h;
        PC++;
        break;
    case 0x6D:
        // ld l, l
        l = l;
        PC++;
        break;
    case 0x6E:
        // ld l, (hl)
        l = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x6F:
        // ld l, a
        l = a;
        PC++;
        break;
    case 0x70:
        // ld (hl), b
        RAM[combineRegisters(h, l)] = b;
        PC++;
        break;
    case 0x71:
        // ld (hl), c
        RAM[combineRegisters(h, l)] = c;
        PC++;
        break;
    case 0x72:
        // ld (hl), d
        RAM[combineRegisters(h, l)] = d;
        PC++;
        break;
    case 0x73:
        // ld (hl), h
        RAM[combineRegisters(h, l)] = h;
        PC++;
        break;
    case 0x74:
        // ld (hl), l
        RAM[combineRegisters(h, l)] = l;
        PC++;
        break;
    case 0x75:
        // ld (hl), a
        RAM[combineRegisters(h, l)] = a;
        PC++;
        break;
    case 0x76:
        // HALT
        //TODO: Implement halt
        PC++;
        break;
    case 0x77:
        // ld (hl), a
        RAM[combineRegisters(h, l)] = a;
        PC++;
        break;
    case 0x78:
        // ld a, b
        a = b;
        PC++;
        break;
    case 0x79:
        // ld a, c
        a = c;
        PC++;
        break;
    case 0x7A:
        // ld a, d
        a = d;
        PC++;
        break;
    case 0x7B:
        // ld a, e
        a = e;
        PC++;
        break;
    case 0x7C:
        // ld a, h
        a = h;
        PC++;
        break;
    case 0x7D:
        // ld a, l
        a = l;
        PC++;
        break;
    case 0x7E:
        // ld a, (hl)
        a = RAM[combineRegisters(h, l)];
        PC++;
        break;
    case 0x7F:
        // ld a, a
        a = a;
        PC++;
        break;

    case 0xE0:
        // ld ($FF00 + *), a
        RAM[0xFF00 + RAM[PC + 1]] = a;
        PC += 2;
        break;
    case 0xF0:
        // ld a, ($FF00 + *)
        a = RAM[0xFF00 + RAM[PC + 1]];
        PC += 2;
        break;

    case 0xE2:
        // ld ($FF00 + C), A
        RAM[0xFF00 + c] = a;
        PC++;
        break;
    case 0xF2:
        // ld ($FF00 + C), A
        a = RAM[0xFF00 + c];
        PC++;
        break;
    case 0xF8:
        // LD HL,SP+r8
    {
        uint16_t tmp = SP + RAM[PC + 1];
        l = tmp | 0xff00;
        h = (tmp | 0x00ff) >> 8;
        PC += 2;
    }
    break;
    case 0xF9:
        // LD SP,HL
        SP = h << 8;
        SP += l;
        PC += 1;
        break;
    case 0xEA:
        // LD (a16),A
        RAM[fetchNext2BytesInverted(PC)] = a;
        PC += 3;
        break;
    case 0xFA:
        // LD A,(a16)
        a = RAM[fetchNext2BytesInverted(PC)];
        PC += 3;
        break;

        // BITS EXTENSIONS **************************************
    case 0xCB:
        PC++;
        bitExtensions(RAM[PC]);
        break;
    default:
        break;
    }
}

uint16_t CPU::fetchNext2BytesInverted(int PC)
{
    return (RAM[PC + 1] | 0xff00) & ((RAM[PC + 2] << 8) | 0x00ff);
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

void CPU::increaseRegister(uint8_t* reg1, uint8_t* reg2)
{

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

void CPU::decreaseRegister(uint8_t* reg1, uint8_t* reg2)
{
    (*reg2)--;

    if (*reg2 == 0xff)
        (*reg1)--;
}

void CPU::bitExtensions(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x4C:
        // bit 7, h
        setN(false);
        setH(true);
        setZ(!(h & 0x80) == 0x80);
        PC++;
        break;
    default:
        break;
    }
}

void CPU::compare(uint8_t value)
{
    int result = a - value;
    setZ(a == 0);
    setN(true);
    setH(a < 0);
    setC(a < 0);
}
