#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "CPU.h"
#include <stdio.h>
#include <algorithm>

bool showDebug = false;

FILE* pFile;

CPU::CPU(RAM &ram)
{
    PC = 0x00;
    SP = 0;

    a = b = c = d = e = f = h = l = 0;

    //RAM = new uint8_t[0xFFFF];
    this->ram = &ram;

    /*Rom bootRom = Rom("dmg0_rom.bin");

    auto romSize = std::clamp(static_cast<int>(rom.romBuffer.size()), 0, 0x7FFF);
    auto bootRomSize = std::clamp(static_cast<int>(bootRom.romBuffer.size()), 0, 0x7FFF);
    std::memcpy(RAM, &rom.romBuffer[0], romSize);
    std::memcpy(RAM, &bootRom.romBuffer[0], bootRomSize);*/

    //pFile = fopen("babyboy.log", "w");
}

CPU::~CPU()
{
    //fclose(pFile);
}

void CPU::ExecuteInstruction(uint8_t instruction)
{
    //if (showDebug || PC == 0x8F)
    {
        // 0000: 31 A:00 B:00 C:00 D:00 E:00 F:00 H:00 L:00 LY:00 SP:00  Cy:8
        printf("%04x: %02x A:%02x B:%02x C:%02x D:%02x E:%02x F:%02x H:%02x L:%02x LY:%02x SP:%04x\n",
            PC, instruction, a, b, c, d, e, f, h, l, 0, SP);
        showDebug = true;
    }

    switch (instruction)
    {
    case 0x00:
        PC++;
        break;

        // ARITHMETIC/LOGICAL OPERATIONS *****************************************
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
    case 0x09:
        // add hl, bc
        addHL(combineRegisters(b, c));

        PC++;
        break;
    case 0x19:
        // add hl, de
        addHL(combineRegisters(d, e));

        PC++;
        break;
    case 0x29:
        // add hl, hl
        addHL(combineRegisters(h, l));

        PC++;
        break;
    case 0x39:
        // add hl, sp
        addHL(SP);

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
    case 0x34:
        // inc (hl)
        increaseMemoryAddress(combineRegisters(h, l));
        PC++;
        break;
    case 0x35:
        // dec (hl)
        decreaseMemoryAddress(combineRegisters(h, l));
        PC++;
        break;
    case 0x37:
        // scf
        setN(false);
        setH(false);
        setC(true);
        PC++;
        break;
    case 0x2F:
        // cpl
        a = ~a;
        setN(true);
        setH(true);
        PC++;
        break;
    case 0x3F:
        // ccf
        setN(false);
        setH(false);
        setC(!c);
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
    case 0x07:
        //dec bc
        //decreaseRegister(&b);
        //TODO: implement this
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
    case 0x80:
        // add a,b
        add(b);
        PC++;
        break;
    case 0x81:
        // add a,c
        add(c);
        PC++;
        break;
    case 0x82:
        // add a,d
        add(d);
        PC++;
        break;
    case 0x83:
        // add a,e
        add(e);
        PC++;
        break;
    case 0x84:
        // add a,h
        add(h);
        PC++;
        break;
    case 0x85:
        // add a,l
        add(l);
        PC++;
        break;
    case 0x86:
        // add a,(hl)
        if (a == 0x6d)

            add(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0x87:
        // add a,a
        add(a);
        PC++;
        break;
    case 0x88:
        // adc a,b
        adc(b);
        PC++;
        break;
    case 0x89:
        // adc a,c
        adc(c);
        PC++;
        break;
    case 0x8A:
        // adc a,d
        adc(d);
        PC++;
        break;
    case 0x8B:
        // adc a,e
        adc(e);
        PC++;
        break;
    case 0x8C:
        // adc a,h
        adc(h);
        PC++;
        break;
    case 0x8D:
        // adc a,l
        adc(l);
        PC++;
        break;
    case 0x8E:
        // adc a,(hl)
        adc(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0x8F:
        // adc a,a
        adc(a);
        PC++;
        break;
    case 0x90:
        // sub a,b
        sub(b);
        PC++;
        break;
    case 0x91:
        // sub a,c
        sub(c);
        PC++;
        break;
    case 0x92:
        // sub a,d
        sub(d);
        PC++;
        break;
    case 0x93:
        // sub a,e
        sub(e);
        PC++;
        break;
    case 0x94:
        // sub a,h
        sub(h);
        PC++;
        break;
    case 0x95:
        // sub a,l
        sub(l);
        PC++;
        break;
    case 0x96:
        // sub a,(hl)
        sub(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0x97:
        // sub a,a
        sub(a);
        PC++;
        break;
    case 0x98:
        // sbc a,b
        sbc(b);
        PC++;
        break;
    case 0x99:
        // sbc a,c
        sbc(c);
        PC++;
        break;
    case 0x9A:
        // sbc a,d
        sbc(d);
        PC++;
        break;
    case 0x9B:
        // sbc a,e
        sbc(e);
        PC++;
        break;
    case 0x9C:
        // sbc a,h
        sbc(h);
        PC++;
        break;
    case 0x9D:
        // sbc a,l
        sbc(l);
        PC++;
        break;
    case 0x9E:
        // sbc a,(hl)
        sbc(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0x9F:
        // sbc a,a
        sbc(a);
        PC++;
        break;
    case 0xA0:
        // and b
        reg_and(b);
        PC++;
        break;
    case 0xA1:
        // and c
        reg_and(c);
        PC++;
        break;
    case 0xA2:
        // and d
        reg_and(d);
        PC++;
        break;
    case 0xA3:
        // and e
        reg_and(e);
        PC++;
        break;
    case 0xA4:
        // and h
        reg_and(h);
        PC++;
        break;
    case 0xA5:
        // and l
        reg_and(l);
        PC++;
        break;
    case 0xA6:
        // and (hl)
        reg_and(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0xA7:
        // and a
        reg_and(a);
        PC++;
        break;
    case 0xA8:
        // xor b
        reg_xor(b);
        PC++;
        break;
    case 0xA9:
        // xor c
        reg_xor(c);
        PC++;
        break;
    case 0xAA:
        // xor d
        reg_xor(d);
        PC++;
        break;
    case 0xAB:
        // xor e
        reg_xor(e);
        PC++;
        break;
    case 0xAC:
        // xor h
        reg_xor(h);
        PC++;
        break;
    case 0xAD:
        // xor l
        reg_xor(l);
        PC++;
        break;
    case 0xAE:
        // xor (hl)
        reg_xor(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0xAF:
        // xor a
        reg_xor(a);
        PC++;
        break;
    case 0xB0:
        // orb
        reg_or(b);
        PC++;
        break;
    case 0xB1:
        // orc
        reg_or(c);
        PC++;
        break;
    case 0xB2:
        // ord
        reg_or(d);
        PC++;
        break;
    case 0xB3:
        // ore
        reg_or(e);
        PC++;
        break;
    case 0xB4:
        // orh
        reg_or(h);
        PC++;
        break;
    case 0xB5:
        // orl
        reg_or(l);
        PC++;
        break;
    case 0xB6:
        // or(hl)
        reg_or(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0xB7:
        // ora
        reg_or(a);
        PC++;
        break;
    case 0xB8:
        // cp b
        reg_cp(b);
        PC++;
        break;
    case 0xB9:
        // cp c
        reg_cp(c);
        PC++;
        break;
    case 0xBA:
        // cp d
        reg_cp(d);
        PC++;
        break;
    case 0xBB:
        // cp e
        reg_cp(e);
        PC++;
        break;
    case 0xBC:
        // cp h
        reg_cp(h);
        PC++;
        break;
    case 0xBD:
        // cp l
        reg_cp(l);
        PC++;
        break;
    case 0xBE:
        // cp (hl)
        reg_cp(ram->read(combineRegisters(h, l)));
        PC++;
        break;
    case 0xBF:
        // cp a
        reg_cp(a);
        PC++;
        break;
    case 0xC6:
        // add *
        add(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xD6:
        // sub *
        sub(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xE6:
        // and *
        reg_and(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xF6:
        // or *
        reg_or(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xCE:
        // adc *
        adc(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xDE:
        // sbc *
        sbc(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xEE:
        // xor *
        reg_xor(ram->read(PC + 1));
        PC += 2;
        break;
    case 0xFE:
        // cp *

        reg_cp(ram->read(PC + 1));
        PC += 2;
        break;

        // JUMP/CALL OPERATIONS *****************************************
    case 0x20:
        // jr nz, *
        if (!isFlagSet(Flag::Z))
            PC += ram->read(PC + 1);

        PC += 2;
        break;
    case 0x30:
        // jr nc, *
        if (!isFlagSet(Flag::C))
            PC += ram->read(PC + 1);
        PC += 2;
        break;


    case 0x18:
        // jr *
        PC += ram->read(PC + 1);
        PC += 2;
        break;
    case 0x28:
        // jr z, *
        if (isFlagSet(Flag::Z))
            PC += ram->read(PC + 1);

        PC += 2;
        break;
    case 0x38:
        // jr c, *
        if (isFlagSet(Flag::C))
            PC += ram->read(PC + 1);

        PC += 2;
        break;
    case 0xC2:
        // jp nz, **
        PC = !isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xD2:
        // jp nc, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xC3:
        // jp **
        PC = fetchNext2BytesInverted(PC);
        break;
    case 0xCA:
        // jp z, **
        PC = isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xDA:
        // jp c, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        break;
    case 0xE9:
        // jp (hl), **
        PC = receive2bytesFromRam(combineRegisters(h, l));
        break;
    case 0xC0:
        // ret nz
        if (!isFlagSet(Flag::Z))
        {
            PC = fetchAddressFromRam(SP);
        }
        else
            PC += 1;

        SP += 2;
        break;
    case 0xD0:
        // ret nc
        if (!isFlagSet(Flag::C))
        {
            PC = fetchAddressFromRam(SP);
        }
        else
            PC += 1;

        SP += 2;
        break;
    case 0xC8:
        // ret z
        if (isFlagSet(Flag::Z))
        {
            PC = fetchAddressFromRam(SP);
        }
        else
            PC += 1;

        SP += 2;
        break;
    case 0xD8:
        // ret c
        if (isFlagSet(Flag::C))
        {
            PC = fetchAddressFromRam(SP);
        }
        else
            PC += 1;

        SP += 2;
        break;
    case 0xC9:
        // ret
        PC = fetchAddressFromRam(SP);
        SP += 2;
        break;
    case 0xD9:
        // reti
        PC = fetchAddressFromRam(SP);
        SP += 2;
        //TODO ENABLE INTERRUPTS
        break;
    case 0xC7:
        // rst 00h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x00;
        break;
    case 0xD7:
        // rst 10h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x10;
        break;
    case 0xE7:
        // rst 20h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x20;
        break;
    case 0xF7:
        // rst 30h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x30;
        break;
    case 0xCF:
        // rst 08h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x08;
        break;
    case 0xDF:
        // rst 18h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x18;
        break;
    case 0xEF:
        // rst 28h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x28;
        break;
    case 0xFF:
        // rst 38h
        SP += 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x38;
        break;
    case 0xC4:
        // call nz, **
        if (!isFlagSet(Flag::Z))
        {
            SP -= 2;
            add2bytesToRam(SP, PC + 3);
            PC = fetchNext2BytesInverted(PC);
        }
        else
        {
            PC += 1;
        }
        break;
    case 0xD4:
        // call nc, **
        if (!isFlagSet(Flag::C))
        {
            SP -= 2;
            add2bytesToRam(SP, PC + 3);
            PC = fetchNext2BytesInverted(PC);
        }
        else
        {
            PC += 1;
        }
        break;
    case 0xCC:
        // call z, **
        if (isFlagSet(Flag::Z))
        {
            SP -= 2;
            add2bytesToRam(SP, PC + 3);
            PC = fetchNext2BytesInverted(PC);
        }
        else
        {
            PC += 1;
        }
        break;
    case 0xDC:
        // call c, **
        if (isFlagSet(Flag::C))
        {
            SP -= 2;
            add2bytesToRam(SP, PC + 3);
            PC = fetchNext2BytesInverted(PC);
        }
        else
        {
            PC += 1;
        }
        break;
    case 0xCD:
        // call **
        SP -= 2;
        add2bytesToRam(SP, PC + 3);
        PC = fetchNext2BytesInverted(PC);
        break;


        // PUSH/POP OPERATIONS *****************************************
    case 0xC1:
        // pop bc
        c = ram->read(SP++);
        b = ram->read(SP++);
        PC++;
        break;
    case 0xD1:
        // pop DE
        e = ram->read(SP++);
        d = ram->read(SP++);
        PC++;
        break;
    case 0xE1:
        // pop HL
        l = ram->read(SP++);
        h = ram->read(SP++);
        PC++;
        break;
    case 0xF1:
        // pop AF
        f = ram->read(SP++);
        a = ram->read(SP++);
        PC++;
        break;

    case 0xC5:
        // push bc
        ram->write8(--SP, b);
        ram->write8(--SP, c);
        PC++;
        break;
    case 0xD5:
        // push de
        ram->write8(--SP, d);
        ram->write8(--SP, e);
        PC++;
        break;
    case 0xE5:
        // push hl
        ram->write8(--SP, h);
        ram->write8(--SP, l);
        PC++;
        break;
    case 0xF5:
        // push af
        ram->write8(--SP, a);
        ram->write8(--SP, f);
        PC++;
        break;

        // LOAD OPERATIONS *****************************************
    case 0x01:
        // ld bc, **
        c = ram->read(PC + 1);
        b = ram->read(PC + 2);
        PC += 3;
        break;
    case 0x11:
        // ld de, **
        e = ram->read(PC + 1);
        d = ram->read(PC + 2);
        PC += 3;
        break;
    case 0x21:
        // ld hl **
        l = ram->read(PC + 1);
        h = ram->read(PC + 2);
        PC += 3;
        break;
    case 0x31:
        // ld sp **
        SP = fetchNext2BytesInverted(PC);
        PC += 3;
        break;
    case 0x02:
        // ld (bc), a
        ram->write8(combineRegisters(b, c), a);
        PC += 1;
        break;
    case 0x12:
        // ld (de), a
        ram->write8(combineRegisters(d, e), a);
        PC += 1;
        break;
    case 0x22:
        // ldi (hl), a
        ram->write8(combineRegisters(h, l), a);

        l++;
        if (l == 0)
            h++;

        PC++;
        break;
    case 0x32:
        // ldd (hl), a
        ram->write8(combineRegisters(h, l), a);
        decreaseRegister(&h, &l);
        PC++;
        break;
    case 0x06:
        // ld b, *
        b = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x16:
        // ld d, *
        d = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x26:
        // ld h, *
        h = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x36:
        // ld h, *
        h = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x08:
        // ld (a16), SP
        add2bytesToRam(receive2bytesFromRam(fetchNext2BytesInverted(PC)), SP);
        PC += 3;
        break;
    case 0x0A:
        // ld a, (bc)
        a = ram->read(combineRegisters(b, c));
        PC++;
        break;
    case 0x1A:
        // ld a, (de)
        a = ram->read(combineRegisters(d, e));
        PC++;
        break;
    case 0x2A:
        // ldi a, (hl)
        a = ram->read(combineRegisters(h, l));
        increaseRegister(&h, &l);
        PC++;
        break;
    case 0x3A:
        // ldd a, (hl)
        a = ram->read(combineRegisters(h, l));
        decreaseRegister(&h, &l);
        PC++;
        break;
    case 0x0E:
        // ld c, *
        c = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x1E:
        // ld e, *
        e = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x2E:
        // ld l, *
        l = ram->read(PC + 1);
        PC += 2;
        break;
    case 0x3E:
        // ld a, *
        a = ram->read(PC + 1);
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
        b = ram->read(combineRegisters(h, l));
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
        c = ram->read(combineRegisters(h, l));
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
        d = ram->read(combineRegisters(h, l));
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
        e = ram->read(combineRegisters(h, l));
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
        h = ram->read(combineRegisters(h, l));
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
        l = ram->read(combineRegisters(h, l));
        PC++;
        break;
    case 0x6F:
        // ld l, a
        l = a;
        PC++;
        break;
    case 0x70:
        // ld (hl), b
        ram->write8(combineRegisters(h, l), b);
        PC++;
        break;
    case 0x71:
        // ld (hl), c
        ram->write8(combineRegisters(h, l), c);
        PC++;
        break;
    case 0x72:
        // ld (hl), d
        ram->write8(combineRegisters(h, l), d);
        PC++;
        break;
    case 0x73:
        // ld (hl), h
        ram->write8(combineRegisters(h, l), h);
        PC++;
        break;
    case 0x74:
        // ld (hl), l
        ram->write8(combineRegisters(h, l), l);
        PC++;
        break;
    case 0x75:
        // ld (hl), a
        ram->write8(combineRegisters(h, l), a);
        PC++;
        break;
    case 0x76:
        // HALT
        //TODO: Implement halt
        PC++;
        break;
    case 0x77:
        // ld (hl), a
        ram->write8(combineRegisters(h, l), a);
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
        a = ram->read(combineRegisters(h, l));
        PC++;
        break;
    case 0x7F:
        // ld a, a
        a = a;
        PC++;
        break;

    case 0xE0:
        // ld ($FF00 + *), a
        ram->write8(0xFF00 + ram->read(PC + 1), a);
        PC += 2;
        break;
    case 0xF0:
        // ld a, ($FF00 + *)
        a = ram->read(0xFF00 + ram->read(PC + 1));
        PC += 2;
        break;

    case 0xE2:
        // ld ($FF00 + C), A
        ram->write8(0xFF00 + c, a);
        PC++;
        break;
    case 0xF2:
        // ld ($FF00 + C), A
        a = ram->read(0xFF00 + c);
        PC++;
        break;
    case 0xF8:
        // LD HL,SP+r8
    {
        uint16_t tmp = SP + ram->read(PC + 1);
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
        ram->write8(fetchNext2BytesInverted(PC), a);
        PC += 3;
        break;
    case 0xFA:
        // LD A,(a16)
        a = ram->read(fetchNext2BytesInverted(PC));
        PC += 3;
        break;

        // 8 BIT ROTATION ***************************************
    case 0x17:
        // RLA
    {
        auto original_a = a;
        a = a << 1;
        a = a | (getC() ? 1 : 0);
        setC(original_a >> 7);
        setZ(0);
        setH(0);
        setN(0);
        PC++;
    }
    break;

    // BITS EXTENSIONS **************************************
    case 0xCB:
        PC++;
        bitExtensions(ram->read(PC));
        break;

    default:
        printf("Not implemented %x - PC %x\n", instruction, PC);
        break;
    }

    //printf("\n");
}

uint16_t CPU::fetchNext2BytesInverted(int PC)
{
    return (ram->read(PC + 1) | 0xff00) & ((ram->read(PC + 2) << 8) | 0x00ff);
}

uint16_t CPU::receive2bytesFromRam(int ramPos)
{
    uint16_t result;

    result = ram->read(ramPos + 1) << 8;
    result += ram->read(ramPos);

    return result;
}

uint16_t CPU::fetchAddressFromRam(int ramPos)
{
    uint16_t result;

    result = ram->read(ramPos) << 8;
    result += ram->read(ramPos + 1);

    return result;
}

void CPU::add2bytesToRam(int ramPos, uint16_t value)
{
    ram->write8(ramPos, value >> 8);
    ram->write8(ramPos + 1, (uint8_t)value);
}

bool CPU::isFlagSet(Flag flag)
{
    return f & (1 << flag);
}

void CPU::setZ(bool value)
{
    f ^= (-value ^ f) & (1UL << Z);
}

void CPU::setN(bool value)
{
    f ^= (-value ^ f) & (1UL << N);
}

void CPU::setH(bool value)
{
    f ^= (-value ^ f) & (1UL << H);
}

void CPU::setC(bool value)
{
    f ^= (-value ^ f) & (1UL << C);
}

bool CPU::getC()
{
    return isFlagSet(C);
}

uint16_t CPU::combineRegisters(uint8_t reg1, uint8_t reg2)
{
    return (reg1 << 8 | 0x00ff) & (reg2 | 0xff00);
}

void CPU::increaseMemoryAddress(uint16_t pointer)
{
    uint8_t previousValue = ram->read(pointer);
    auto t = ram->read(pointer);
    ram->write8(pointer, t++);
    uint8_t result = ram->read(pointer);

    if (result == 0)
    {
        setZ(true);
        setH(true);
    }
    else
    {
        setZ(false);
        if ((previousValue & 0x0f) == 0x0f)
            setH(true);
        else
            setH(false);
    }

    setN(false);
}

void CPU::decreaseMemoryAddress(uint16_t pointer)
{
    uint8_t previousValue = ram->read(pointer);
    auto t = ram->read(pointer);
    ram->write8(pointer, t--);
    uint8_t result = ram->read(pointer);

    setZ(result == 0);

    if (((uint8_t)(previousValue << 4)) == 0x00)
        setH(true);
    else
        setH(false);

    setN(true);
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

void CPU::addHL(uint16_t reg)
{
    uint16_t hl = combineRegisters(h, l);

    setN(false);
    setC((uint32_t)hl + (uint32_t)reg > 0xffff);
    setH(h + l > 0x0f);

    hl += reg;
    h = (uint8_t)(hl >> 8);
    l = (uint8_t)(hl | 0x1100);
}

void CPU::add(uint8_t reg)
{
    setC((uint16_t)a + (uint16_t)reg > 0xff);
    setH((((a & 0xf) + (reg & 0xf)) & 0x10) == 0x10);

    a = a + reg;

    setZ(a == 0);
    setN(false);
}

void CPU::adc(uint8_t reg)
{
    int carry = getC() == true ? 1 : 0;

    setC((uint16_t)a + (uint16_t)reg + carry > 0xff);
    setH(((uint8_t)a << 4) + ((uint8_t)reg << 4) + carry > 0xff);

    a = a + reg + carry;

    setZ(a == 0);
    setN(false);
}

void CPU::sub(uint8_t reg)
{
    setC((int16_t)a - (int16_t)reg < 0x00);
    setH((a & 0x0f) - (reg & 0x0f) < 0x00);

    a = a - reg;

    setZ(a == 0);
    setN(true);
}

void CPU::sbc(uint8_t reg)
{
    int carry = getC() == true ? 1 : 0;

    setC((int16_t)a - (int16_t)reg - carry < 0x00);
    setH((a & 0x0f) - (reg & 0x0f) - carry < 0x00);

    a = a - reg;

    setZ(a == 0);
    setN(true);
}

void CPU::reg_and(uint8_t reg)
{
    a = a & reg;
    setZ(a == 0);
    setN(false);
    setH(true);
    setC(false);
}

void CPU::reg_xor(uint8_t reg)
{
    a = a ^ reg;
    setZ(a == 0);
    setN(false);
    setH(false);
    setC(false);
}

void CPU::reg_or(uint8_t reg)
{
    a = a | reg;
    setZ(a == 0);
    setN(false);
    setH(false);
    setC(false);
}

void CPU::reg_cp(uint8_t reg)
{
    setC((int16_t)a - (int16_t)reg < 0x00);
    //setH((a & 0x0f) - (reg & 0x0f) < 0x00);
    setH(a - reg > 0x0f);

    setZ(reg - a <= 0);
    setN(true);
}

void CPU::bitExtensions(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x11:
        // rl c
    {
        auto original_c = c;
        c = c << 1;
        c = c | (getC() ? 1 : 0);
        setC(original_c >> 7);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
    }
    break;
    case 0x7C:
        // bit 7, h
        setN(false);
        setH(true);
        setZ(!((h & 0x80) == 0x80));
        PC++;
        break;
    default:
        printf("Not implemented CB %x - PC %x (extension)\n", opcode, PC);
        break;
    }
}
