#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "CPU.h"
#include <stdio.h>
#include <algorithm>

bool showDebug = false;

#ifdef WRITE_LOG
#include <stdio.h>
#include <stdlib.h>
FILE *logFile;
#endif



CPU::CPU(RAM& ram)
{
    PC = 0x00;
    SP = 0;

    a = b = c = d = e = f = h = l = 0;

    cycles = 0;
    lastClockCycle = 0;

    this->ram = &ram;

    

#ifdef WRITE_LOG
    logFile = fopen("babyboy_log","w");
#endif
}

CPU::~CPU()
{
}

void CPU::ExecuteInstruction(uint8_t instruction)
{
    // Debugger
    if (lastInstructions.size() > MAX_INSTRUCTIONS_LIST) {
        lastInstructions.pop_front();
    }

    if (!isOnIllegalInstruction) {
        lastInstructions.push_back(DebugInstruction(PC, instruction, a, b, c, d, e, f, h, l, SP));
    }

#ifdef WRITE_LOG
    //A:00 F:11 B:22 C:33 D:44 E:55 H:66 L:77 SP:8888 PC:9999 PCMEM:AA,BB,CC,DD    

    fprintf(logFile, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
    a, f, b, c, d, e, h, l, SP, PC, ram->read(PC), ram->read(PC+1), ram->read(PC+2), ram->read(PC+3));
    fflush(logFile);
#endif

    switch (instruction)
    {
    case 0x00:
        PC++;
        lastClockCycle = 4;
        break;

        // ARITHMETIC/LOGICAL OPERATIONS *****************************************
    case 0x03:
        // inc bc
        c++;
        if (c == 0)
            b++;

        PC++;
        lastClockCycle = 8;
        break;
    case 0x13:
        // inc de
        e++;
        if (e == 0)
            d++;

        PC++;
        lastClockCycle = 8;
        break;
    case 0x0B:
        //dec bc
        if (c == 0)
        {
            b--;
        }

        c--;
        PC++;
        lastClockCycle = 8;
        break;
    case 0x1B:
        //dec de
        if (e == 0)
        {
            d--;
        }

        e--;
        PC++;
        lastClockCycle = 8;
        break;
    case 0x2B:
        //dec hl
        if (l == 0)
        {
            h--;
        }

        l--;
        PC++;
        lastClockCycle = 8;
        break;
    case 0x3B:
        //dec sp
        SP--;
        PC++;
        lastClockCycle = 8;
        break;
    case 0x09:
        // add hl, bc
        addHL(combineRegisters(b, c));

        PC++;
        lastClockCycle = 8;
        break;
    case 0x19:
        // add hl, de
        addHL(combineRegisters(d, e));

        PC++;
        lastClockCycle = 8;
        break;
    case 0x29:
        // add hl, hl
        addHL(combineRegisters(h, l));

        PC++;
        lastClockCycle = 8;
        break;
    case 0x39:
        // add hl, sp
        addHL(SP);

        PC++;
        lastClockCycle = 8;
        break;

    case 0x23:
        // inc hl
        l++;
        if (l == 0)
            h++;

        PC++;
        lastClockCycle = 8;
        break;
    case 0x33:
        // inc sp
        SP++;
        PC++;
        lastClockCycle = 8;
        break;
    case 0x34:
        // inc (hl)
        increaseMemoryAddress(combineRegisters(h, l));
        PC++;
        lastClockCycle = 12;
        break;
    case 0x35:
        // dec (hl)
        decreaseMemoryAddress(combineRegisters(h, l));
        PC++;
        lastClockCycle = 12;
        break;
    case 0x37:
        // scf
        setN(false);
        setH(false);
        setC(true);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x2F:
        // cpl
        a = ~a;
        setN(true);
        setH(true);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x3F:
        // ccf
        setN(false);
        setH(false);
        setC(!c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x04:
        //inc b
        increaseRegister(&b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x05:
        //dec b
        decreaseRegister(&b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x0C:
        //inc C
        increaseRegister(&c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x0D:
        //dec C
        decreaseRegister(&c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x14:
        //inc D
        increaseRegister(&d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x15:
        //dec D
        decreaseRegister(&d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x24:
        //inc H
        increaseRegister(&h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x25:
        //dec H
        decreaseRegister(&h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x1C:
        //inc E
        increaseRegister(&e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x1D:
        //dec E
        decreaseRegister(&e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x2C:
        //inc L
        increaseRegister(&l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x2D:
        //dec L
        decreaseRegister(&l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x3C:
        //inc A
        increaseRegister(&a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x3D:
        //dec A
        decreaseRegister(&a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x80:
        // add a,b
        add(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x81:
        // add a,c
        add(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x82:
        // add a,d
        add(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x83:
        // add a,e
        add(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x84:
        // add a,h
        add(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x85:
        // add a,l
        add(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x86:
        // add a,(hl)
        add(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x87:
        // add a,a
        add(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x88:
        // adc a,b
        adc(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x89:
        // adc a,c
        adc(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x8A:
        // adc a,d
        adc(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x8B:
        // adc a,e
        adc(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x8C:
        // adc a,h
        adc(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x8D:
        // adc a,l
        adc(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x8E:
        // adc a,(hl)
        adc(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x8F:
        // adc a,a
        adc(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x90:
        // sub a,b
        sub(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x91:
        // sub a,c
        sub(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x92:
        // sub a,d
        sub(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x93:
        // sub a,e
        sub(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x94:
        // sub a,h
        sub(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x95:
        // sub a,l
        sub(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x96:
        // sub a,(hl)
        sub(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x97:
        // sub a,a
        sub(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x98:
        // sbc a,b
        sbc(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x99:
        // sbc a,c
        sbc(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x9A:
        // sbc a,d
        sbc(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x9B:
        // sbc a,e
        sbc(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x9C:
        // sbc a,h
        sbc(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x9D:
        // sbc a,l
        sbc(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0x9E:
        // sbc a,(hl)
        sbc(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x9F:
        // sbc a,a
        sbc(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA0:
        // and b
        reg_and(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA1:
        // and c
        reg_and(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA2:
        // and d
        reg_and(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA3:
        // and e
        reg_and(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA4:
        // and h
        reg_and(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA5:
        // and l
        reg_and(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA6:
        // and (hl)
        reg_and(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0xA7:
        // and a
        reg_and(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA8:
        // xor b
        reg_xor(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xA9:
        // xor c
        reg_xor(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xAA:
        // xor d
        reg_xor(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xAB:
        // xor e
        reg_xor(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xAC:
        // xor h
        reg_xor(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xAD:
        // xor l
        reg_xor(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xAE:
        // xor (hl)
        reg_xor(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0xAF:
        // xor a
        reg_xor(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB0:
        // orb
        reg_or(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB1:
        // orc
        reg_or(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB2:
        // ord
        reg_or(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB3:
        // ore
        reg_or(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB4:
        // orh
        reg_or(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB5:
        // orl
        reg_or(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB6:
        // or(hl)
        reg_or(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0xB7:
        // ora
        reg_or(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB8:
        // cp b
        reg_cp(b);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xB9:
        // cp c
        reg_cp(c);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xBA:
        // cp d
        reg_cp(d);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xBB:
        // cp e
        reg_cp(e);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xBC:
        // cp h
        reg_cp(h);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xBD:
        // cp l
        reg_cp(l);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xBE:
        // cp (hl)
        reg_cp(ram->read(combineRegisters(h, l)));
        PC++;
        lastClockCycle = 8;
        break;
    case 0xBF:
        // cp a
        reg_cp(a);
        PC++;
        lastClockCycle = 4;
        break;
    case 0xC6:
        // add *
        add(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xD6:
        // sub *
        sub(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xE6:
        // and *
        reg_and(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xF6:
        // or *
        reg_or(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xCE:
        // adc *
        adc(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xDE:
        // sbc *
        sbc(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xEE:
        // xor *
        reg_xor(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0xFE:
        // cp *

        reg_cp(ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 8;
        break;

        // JUMP/CALL OPERATIONS *****************************************
    case 0x20:
        // jr nz, *
        if (!isFlagSet(Flag::Z)) {
            PC += (int8_t)ram->read(PC + 1);
            lastClockCycle = 12;
        } else {
            lastClockCycle = 8;
        }

        PC += 2;
        break;
    case 0x30:
        // jr nc, *
        if (!isFlagSet(Flag::C)) {
            PC += (int8_t)ram->read(PC + 1);
            lastClockCycle = 12;
        } else {
            lastClockCycle = 8;
        }

        PC += 2;
        break;
    case 0x18:
        // jr *
        PC += (int8_t)ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 12;
        break;
    case 0x28:
        // jr z, *
        if (isFlagSet(Flag::Z))
            PC += (int8_t)ram->read(PC + 1);

        PC += 2;
        lastClockCycle = 12;
        break;
    case 0x38:
        // jr c, *
        if (isFlagSet(Flag::C))
            PC += (int8_t)ram->read(PC + 1);

        PC += 2;
        lastClockCycle = 12;
        break;
    case 0xC2:
        // jp nz, **
        PC = !isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        lastClockCycle = 16;
        break;
    case 0xD2:
        // jp nc, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        lastClockCycle = 16;
        break;
    case 0xC3:
        // jp **
        PC = fetchNext2BytesInverted(PC);
        lastClockCycle = 16;
        break;
    case 0xCA:
        // jp z, **
        PC = isFlagSet(Flag::Z) ? fetchNext2BytesInverted(PC) : PC += 3;
        lastClockCycle = 16;
        break;
    case 0xDA:
        // jp c, **
        PC = !isFlagSet(Flag::C) ? fetchNext2BytesInverted(PC) : PC += 3;
        lastClockCycle = 16;
        break;
    case 0xE9:
        // jp hl
        PC = combineRegisters(h, l);
        lastClockCycle = 4;
        break;
    case 0xC0:
        // ret nz
        if (!isFlagSet(Flag::Z))
        {
            PC = fetchAddressFromRam(SP);
            SP += 2;
        }
        else
            PC += 1;

        lastClockCycle = 20;
        break;
    case 0xD0:
        // ret nc
        if (!isFlagSet(Flag::C))
        {
            PC = fetchAddressFromRam(SP);
            SP += 2;
        }
        else
            PC += 1;

        lastClockCycle = 20;
        break;
    case 0xC8:
        // ret z
        if (isFlagSet(Flag::Z))
        {
            PC = fetchAddressFromRam(SP);
            SP += 2;
        }
        else
            PC += 1;

        lastClockCycle = 20;
        break;
    case 0xD8:
        // ret c
        if (isFlagSet(Flag::C))
        {
            PC = fetchAddressFromRam(SP);
            SP += 2;
        }
        else
            PC += 1;

        lastClockCycle = 20;
        break;
    case 0xC9:
        // ret
        PC = fetchAddressFromRam(SP);
        SP += 2;
        lastClockCycle = 16;
        break;
    case 0xD9:
        // reti
        PC = fetchAddressFromRam(SP);
        SP += 2;
        //TODO ENABLE INTERRUPTS
        lastClockCycle = 16;
        break;
    case 0xC7:
        // rst 00h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x00;
        lastClockCycle = 16;
        break;
    case 0xD7:
        // rst 10h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x10;
        lastClockCycle = 16;
        break;
    case 0xE7:
        // rst 20h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x20;
        lastClockCycle = 16;
        break;
    case 0xF7:
        // rst 30h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x30;
        lastClockCycle = 16;
        break;
    case 0xCF:
        // rst 08h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x08;
        lastClockCycle = 16;
        break;
    case 0xDF:
        // rst 18h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x18;
        lastClockCycle = 16;
        break;
    case 0xEF:
        // rst 28h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x28;
        lastClockCycle = 16;
        break;
    case 0xFF:
        // rst 38h
        SP -= 2;
        add2bytesToRam(SP, PC + 1);
        PC = 0x38;
        lastClockCycle = 16;
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
            PC += 3;
        }
        lastClockCycle = 24;
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
            PC += 3;
        }
        lastClockCycle = 24;
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
            PC += 3;
        }
        lastClockCycle = 24;
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
            PC += 3;
        }
        lastClockCycle = 24;
        break;
    case 0xCD:
        // call **
        SP -= 2;
        add2bytesToRam(SP, PC + 3);
        PC = fetchNext2BytesInverted(PC);
        lastClockCycle = 24;
        break;


        // PUSH/POP OPERATIONS *****************************************
    case 0xC1:
        // pop bc
        c = ram->read(SP++);
        b = ram->read(SP++);
        PC++;
        lastClockCycle = 12;
        break;
    case 0xD1:
        // pop DE
        e = ram->read(SP++);
        d = ram->read(SP++);
        PC++;
        lastClockCycle = 12;
        break;
    case 0xE1:
        // pop HL
        l = ram->read(SP++);
        h = ram->read(SP++);
        PC++;
        lastClockCycle = 12;
        break;
    case 0xF1:
        // pop AF
        f = ram->read(SP++);
        a = ram->read(SP++);
        PC++;
        lastClockCycle = 12;
        break;

    case 0xC5:
        // push bc
        ram->write8(--SP, b);
        ram->write8(--SP, c);
        PC++;
        lastClockCycle = 16;
        break;
    case 0xD5:
        // push de
        ram->write8(--SP, d);
        ram->write8(--SP, e);
        PC++;
        lastClockCycle = 16;
        break;
    case 0xE5:
        // push hl
        ram->write8(--SP, h);
        ram->write8(--SP, l);
        PC++;
        lastClockCycle = 16;
        break;
    case 0xF5:
        // push af
        ram->write8(--SP, a);
        ram->write8(--SP, f);
        PC++;
        lastClockCycle = 16;
        break;

        // LOAD OPERATIONS *****************************************
    case 0x01:
        // ld bc, **
        c = ram->read(PC + 1);
        b = ram->read(PC + 2);
        PC += 3;
        lastClockCycle = 12;
        break;
    case 0x11:
        // ld de, **
        e = ram->read(PC + 1);
        d = ram->read(PC + 2);
        PC += 3;
        lastClockCycle = 12;
        break;
    case 0x21:
        // ld hl **
        l = ram->read(PC + 1);
        h = ram->read(PC + 2);
        PC += 3;
        lastClockCycle = 12;
        break;
    case 0x31:
        // ld sp **
        SP = fetchNext2BytesInverted(PC);
        PC += 3;
        lastClockCycle = 12;
        break;
    case 0x02:
        // ld (bc), a
        ram->write8(combineRegisters(b, c), a);
        PC += 1;
        lastClockCycle = 8;
        break;
    case 0x12:
        // ld (de), a
        ram->write8(combineRegisters(d, e), a);
        PC += 1;
        lastClockCycle = 8;
        break;
    case 0x22:
        // ldi (hl), a
        ram->write8(combineRegisters(h, l), a);

        l++;
        if (l == 0)
            h++;

        PC++;
        lastClockCycle = 8;
        break;
    case 0x32:
        // ldd (hl), a
        ram->write8(combineRegisters(h, l), a);
        decreaseRegister(&h, &l);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x06:
        // ld b, *
        b = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x16:
        // ld d, *
        d = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x26:
        // ld h, *
        h = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x36:
        // ld h, *
        h = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 12;
        break;
    case 0x08:
        // ld (a16), SP
        add2bytesToRam(receive2bytesFromRam(fetchNext2BytesInverted(PC)), SP);
        PC += 3;
        lastClockCycle = 20;
        break;
    case 0x0A:
        // ld a, (bc)
        a = ram->read(combineRegisters(b, c));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x1A:
        // ld a, (de)
        a = ram->read(combineRegisters(d, e));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x2A:
        // ldi a, (hl)
        a = ram->read(combineRegisters(h, l));
        increaseRegister(&h, &l);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x3A:
        // ldd a, (hl)
        a = ram->read(combineRegisters(h, l));
        decreaseRegister(&h, &l);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x0E:
        // ld c, *
        c = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x1E:
        // ld e, *
        e = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x2E:
        // ld l, *
        l = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x3E:
        // ld a, *
        a = ram->read(PC + 1);
        PC += 2;
        lastClockCycle = 8;
        break;
    case 0x40:
        // ld b, b
        b = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x41:
        // ld b, c
        b = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x42:
        // ld b, d
        b = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x43:
        // ld b, e
        b = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x44:
        // ld b, h
        b = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x45:
        // ld b, l
        b = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x46:
        // ld b, (hl)
        b = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x47:
        // ld b, a
        b = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x48:
        // ld c, b
        c = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x49:
        // ld c, c
        c = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x4A:
        // ld c, d
        c = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x4B:
        // ld c, e
        c = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x4C:
        // ld c, h
        c = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x4D:
        // ld c, l
        c = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x4E:
        // ld c, (hl)
        c = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x4F:
        // ld c, a
        c = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x50:
        // ld d, b
        d = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x51:
        // ld d, c
        d = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x52:
        // ld d, d
        d = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x53:
        // ld d, e
        d = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x54:
        // ld d, h
        d = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x55:
        // ld d, l
        d = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x56:
        // ld d, (hl)
        d = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x57:
        // ld d, a
        d = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x58:
        // ld e, b
        e = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x59:
        // ld e, c
        e = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x5A:
        // ld e, d
        e = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x5B:
        // ld e, e
        e = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x5C:
        // ld e, h
        e = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x5D:
        // ld e, l
        e = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x5E:
        // ld e, (hl)
        e = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x5F:
        // ld e, a
        e = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x60:
        // ld h, b
        h = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x61:
        // ld h, c
        h = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x62:
        // ld h, d
        h = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x63:
        // ld h, e
        h = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x64:
        // ld h, h
        h = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x65:
        // ld h, l
        h = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x66:
        // ld h, (hl)
        h = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x67:
        // ld h, a
        h = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x68:
        // ld l, b
        l = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x69:
        // ld l, c
        l = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x6A:
        // ld l, d
        l = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x6B:
        // ld l, e
        l = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x6C:
        // ld l, h
        l = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x6D:
        // ld l, l
        l = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x6E:
        // ld l, (hl)
        l = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x6F:
        // ld l, a
        l = a;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x70:
        // ld (hl), b
        ram->write8(combineRegisters(h, l), b);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x71:
        // ld (hl), c
        ram->write8(combineRegisters(h, l), c);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x72:
        // ld (hl), d
        ram->write8(combineRegisters(h, l), d);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x73:
        // ld (hl), e
        ram->write8(combineRegisters(h, l), e);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x74:
        // ld (hl), l
        ram->write8(combineRegisters(h, l), h);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x75:
        // ld (hl), a
        ram->write8(combineRegisters(h, l), l);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x77:
        // ld (hl), a
        ram->write8(combineRegisters(h, l), a);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x78:
        // ld a, b
        a = b;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x79:
        // ld a, c
        a = c;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x7A:
        // ld a, d
        a = d;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x7B:
        // ld a, e
        a = e;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x7C:
        // ld a, h
        a = h;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x7D:
        // ld a, l
        a = l;
        PC++;
        lastClockCycle = 4;
        break;
    case 0x7E:
        // ld a, (hl)
        a = ram->read(combineRegisters(h, l));
        PC++;
        lastClockCycle = 8;
        break;
    case 0x7F:
        // ld a, a
        a = a;
        PC++;
        lastClockCycle = 4;
        break;

    case 0xE0:
        // ld ($FF00 + *), a
        ram->write8(0xFF00 + ram->read(PC + 1), a);
        PC += 2;
        lastClockCycle = 12;
        break;
    case 0xF0:
        // ld a, ($FF00 + *)
        a = ram->read(0xFF00 + ram->read(PC + 1));
        PC += 2;
        lastClockCycle = 12;
        break;

    case 0xE2:
        // ld ($FF00 + C), A
        ram->write8(0xFF00 + c, a);
        PC++;
        lastClockCycle = 8;
        break;
    case 0xF2:
        // ld ($FF00 + C), A
        a = ram->read(0xFF00 + c);
        PC++;
        lastClockCycle = 8;
        break;
    case 0xF8:
        // LD HL,SP+r8
    {
        uint16_t tmp = SP + ram->read(PC + 1);
        l = tmp | 0xff00;
        h = (tmp | 0x00ff) >> 8;
        PC += 2;
        lastClockCycle = 12;
        break;
    }

    case 0xF9:
        // LD SP,HL
        SP = h << 8;
        SP += l;
        PC += 1;
        lastClockCycle = 8;
        break;
    case 0xEA:
        // LD (a16),A
        ram->write8(fetchNext2BytesInverted(PC), a);
        PC += 3;
        lastClockCycle = 16;
        break;
    case 0xFA:
        // LD A,(a16)
        a = ram->read(fetchNext2BytesInverted(PC));
        PC += 3;
        lastClockCycle = 16;
        break;

        // 8 BIT ROTATION ***************************************
    case 0x07:
        // RLCA
    {
        auto original_a = a;
        a = a << 1;
        a = a | (original_a >> 7);
        setC(original_a >> 7);
        setZ(0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 4;
        break;
    }
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
        lastClockCycle = 4;
        break;
    }
    case 0x0F:
        // RRCA
    {
        auto bit0 = a & 0b00000001;
        a = a >> 1;
        a = modifyBit(a, 7, bit0);
        setC(bit0);
        setZ(0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 4;
        break;
    }
    case 0x1F:
        // RRA
    {
        auto bit0 = a & 0b00000001;
        auto previousC = getC();
        a = a >> 1;
        a = modifyBit(a, 7, previousC);
        setC(bit0);
        setZ(0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 4;
        break;
    }
    // CPU CONTROL
    case 0x76:
        // HALT
        //TODO: Implement halt
        PC++;
        lastClockCycle = 4;
        break;
    case 0xF3:
        // DI
        //TODO: Implement DI
        PC++;
        lastClockCycle = 4;
        break;
    case 0xFB:
        // EI
        //TODO: Implement EI
        PC++;
        lastClockCycle = 4;
        break;

        // BITS EXTENSIONS **************************************
    case 0xCB:
        PC++;
        bitExtensions(ram->read(PC));
        break;

    default:
        if (!isOnIllegalInstruction)
            printf("Not implemented %x - PC %x\n", instruction, PC);

        isOnIllegalInstruction = true;
        break;
    }

    //printf("\n");
}

uint8_t CPU::modifyBit(uint8_t number, uint8_t position, uint8_t value)
{
    int mask = 1 << position;
    return (number & ~mask) | ((value << position) & mask);
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

    result = ram->read(ramPos + 1) << 8;
    result += ram->read(ramPos);

    return result;
}

void CPU::add2bytesToRam(int ramPos, uint16_t value)
{
    ram->write8(ramPos + 1, value >> 8);
    ram->write8(ramPos, (uint8_t)value);
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
    ram->write8(pointer, --t);
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
    setH((*reg & 0x0F) == 0x0F);
    setN(0);
    (*reg)++;
    setZ(*reg == 0);
}

void CPU::increaseRegister(uint8_t* reg1, uint8_t* reg2)
{
    if (*reg2 == 0xff) {
        (*reg1)++;
    }

    (*reg2)++;
}


void CPU::decreaseRegister(uint8_t* reg)
{
    setH((*reg & 0x0F) == 0);

    (*reg)--;

    if (*reg == 0)
        setZ(true);
    else
        setZ(false);

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
    setC(((uint32_t)hl + (uint32_t)reg) & 0x10000);
    setH(((hl & 0xFFF) + (reg & 0xFFF)) & 0x1000);

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
    setH((a & 0xf) + (reg & 0xf) + carry > 0x0f);

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
    setH((a & 0x0f) - (reg & 0x0f) < 0x00);
    // setH((a & 0xF) < (reg & 0xF));

    setZ(reg - a == 0);
    setN(true);
}

void CPU::bitExtensions(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x00:
        // rlc b
    {
        auto original_b = b;
        b = b << 1;
        b = b | (original_b >> 7);
        setC(original_b >> 7);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x01:
        // rlc c
    {
        auto original_c = c;
        c = c << 1;
        c = c | (original_c >> 7);
        setC(original_c >> 7);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x02:
        // rlc d
    {
        auto original_d = d;
        d = d << 1;
        d = d | (original_d >> 7);
        setC(original_d >> 7);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x03:
        // rlc e
    {
        auto original_e = e;
        e = e << 1;
        e = e | (original_e >> 7);
        setC(original_e >> 7);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x04:
        // rlc h
    {
        auto original_h = h;
        h = h << 1;
        h = h | (original_h >> 7);
        setC(original_h >> 7);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x05:
        // rlc l
    {
        auto original_l = l;
        l = l << 1;
        l = l | (original_l >> 7);
        setC(original_l >> 7);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x06:
        // rlc (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto hl = original_hl;
        hl = hl << 1;
        hl = hl | (original_hl >> 7);
        setC(original_hl >> 7);
        setZ(hl == 0);
        setH(0);
        setN(0);
        ram->write8(combineRegisters(h, l), hl);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x07:
        // rlc a
    {
        auto original_a = a;
        a = a << 1;
        a = a | (original_a >> 7);
        setC(original_a >> 7);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x08:
        // rrc b
    {
        auto original_b = b;
        b = b >> 1;
        b = b | (original_b & 0b00000001);
        setC(original_b & 0b00000001);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x09:
        // rrc c
    {
        auto original_c = c;
        c = c >> 1;
        c = c | (original_c & 0b00000001);
        setC(original_c & 0b00000001);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x0A:
        // rrc d
    {
        auto original_d = d;
        d = d >> 1;
        d = d | (original_d & 0b00000001);
        setC(original_d & 0b00000001);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x0B:
        // rrc e
    {
        auto original_e = e;
        e = e >> 1;
        e = e | (original_e & 0b00000001);
        setC(original_e & 0b00000001);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x0C:
        // rrc h
    {
        auto original_h = h;
        h = h >> 1;
        h = h | (original_h & 0b00000001);
        setC(original_h & 0b00000001);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x0D:
        // rrc l
    {
        auto original_l = l;
        l = l >> 1;
        l = l | (original_l & 0b00000001);
        setC(original_l & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x0E:
        // rrc (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto hl = original_hl;
        hl = hl >> 1;
        hl = hl | (original_hl & 0b00000001);
        setC(original_hl & 0b00000001);
        setZ(hl == 0);
        setH(0);
        setN(0);
        ram->write8(combineRegisters(h, l), hl);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x0F:
        // rrc a
    {
        auto original_a = a;
        a = a >> 1;
        a = a | (original_a & 0b00000001);
        setC(original_a & 0b00000001);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
    }
    break;

    case 0x10:
        // rl b
    {
        auto original_b = b;
        b = b << 1;
        b = b | (getC() ? 1 : 0);
        setC(original_b >> 7);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
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
        lastClockCycle = 8;
    }
    break;
    case 0x12:
        // rl d
    {
        auto original_d = d;
        d = d << 1;
        d = d | (getC() ? 1 : 0);
        setC(original_d >> 7);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x13:
        // rl e
    {
        auto original_e = e;
        e = e << 1;
        e = e | (getC() ? 1 : 0);
        setC(original_e >> 7);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x14:
        // rl h
    {
        auto original_h = h;
        h = h << 1;
        h = h | (getC() ? 1 : 0);
        setC(original_h >> 7);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x15:
        // rl l
    {
        auto original_l = l;
        l = l << 1;
        l = l | (getC() ? 1 : 0);
        setC(original_l >> 7);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x16:
        // rl (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto temp_hl = ram->read(combineRegisters(h, l));
        temp_hl = temp_hl << 1;
        temp_hl = temp_hl | (getC() ? 1 : 0);
        ram->write8(combineRegisters(h, l), temp_hl);
        setC(original_hl >> 7);
        setZ(temp_hl == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x17:
        // rl a
    {
        auto original_a = a;
        a = a << 1;
        a = a | (getC() ? 1 : 0);
        setC(original_a >> 7);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x18:
        // rr b
    {
        auto original_b = b;
        b = b >> 1;
        b = modifyBit(b, 7, getC());
        setC(original_b & 0b00000001);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x19:
        // rr c
    {
        auto original_c = c;
        c = c >> 1;
        c = modifyBit(c, 7, getC());
        setC(original_c & 0b00000001);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x1A:
        // rr d
    {
        auto original_d = d;
        d = d >> 1;
        d = modifyBit(d, 7, getC());
        setC(original_d & 0b00000001);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x1B:
        // rr e
    {
        auto original_e = e;
        e = e >> 1;
        e = modifyBit(e, 7, getC());
        setC(original_e & 0b00000001);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x1C:
        // rr h
    {
        auto original_h = h;
        h = h >> 1;
        h = modifyBit(h, 7, getC());
        setC(original_h & 0b00000001);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x1D:
        // rr l
    {
        auto original_l = l;
        l = l >> 1;
        l = modifyBit(l, 7, getC());
        setC(original_l & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x1E:
        // rr (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto hl = original_hl;
        hl = hl >> 1;
        hl = modifyBit(hl, 7, getC());
        setC(original_hl & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        ram->write8(combineRegisters(h, l), hl);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x1F:
        // rr a
    {
        auto original_a = a;
        a = a >> 1;
        a = modifyBit(a, 7, getC());
        setC(original_a & 0b00000001);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x20:
        // sla b
    {
        auto original_b = b;
        b = b << 1;
        setC(original_b >> 7);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x21:
        // sla c
    {
        auto original_c = c;
        c = c << 1;
        setC(original_c >> 7);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x22:
        // sla d
    {
        auto original_d = d;
        d = d << 1;
        setC(original_d >> 7);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x23:
        // sla e
    {
        auto original_e = e;
        e = e << 1;
        setC(original_e >> 7);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x24:
        // sla h
    {
        auto original_h = h;
        h = h << 1;
        setC(original_h >> 7);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x25:
        // sla l
    {
        auto original_l = l;
        l = l << 1;
        setC(original_l >> 7);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x26:
        // sla (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto temp_hl = ram->read(combineRegisters(h, l));
        temp_hl = temp_hl << 1;
        ram->write8(combineRegisters(h, l), temp_hl);
        setC(original_hl >> 7);
        setZ(temp_hl == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x27:
        // sla a
    {
        auto original_a = a;
        a = a << 1;
        setC(original_a >> 7);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x28:
        // sra b
    {
        auto original_b = b;
        b = b >> 1;
        b = modifyBit(b, 8, original_b >> 7);
        setC(original_b & 0b00000001);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x29:
        // sra c
    {
        auto original_c = c;
        c = c >> 1;
        c = modifyBit(c, 8, original_c >> 7);
        setC(original_c & 0b00000001);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x2A:
        // sra d
    {
        auto original_d = d;
        d = d >> 1;
        d = modifyBit(d, 8, original_d >> 7);
        setC(original_d & 0b00000001);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x2B:
        // sra e
    {
        auto original_e = e;
        e = e >> 1;
        e = modifyBit(e, 8, original_e >> 7);
        setC(original_e & 0b00000001);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x2C:
        // sra h
    {
        auto original_h = h;
        h = h >> 1;
        h = modifyBit(h, 8, original_h >> 7);
        setC(original_h & 0b00000001);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x2D:
        // sra l
    {
        auto original_l = l;
        l = l >> 1;
        l = modifyBit(l, 8, original_l >> 7);
        setC(original_l & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x2E:
        // sra (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto hl = original_hl;
        hl = hl >> 1;
        hl = modifyBit(hl, 8, original_hl >> 7);
        setC(original_hl & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        ram->write8(combineRegisters(h, l), hl);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x2F:
        // sra a
    {
        auto original_a = a;
        a = a >> 1;
        a = modifyBit(a, 8, original_a >> 7);
        setC(original_a & 0b00000001);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x38:
        // srl b
    {
        auto original_b = b;
        b = b >> 1;
        setC(original_b & 0b00000001);
        setZ(b == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x39:
        // srl c
    {
        auto original_c = c;
        c = c >> 1;
        setC(original_c & 0b00000001);
        setZ(c == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x3A:
        // srl d
    {
        auto original_d = d;
        d = d >> 1;
        setC(original_d & 0b00000001);
        setZ(d == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x3B:
        // srl e
    {
        auto original_e = e;
        e = e >> 1;
        setC(original_e & 0b00000001);
        setZ(e == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x3C:
        // srl h
    {
        auto original_h = h;
        h = h >> 1;
        setC(original_h & 0b00000001);
        setZ(h == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x3D:
        // srl l
    {
        auto original_l = l;
        l = l >> 1;
        setC(original_l & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;
    case 0x3E:
        // srl (hl)
    {
        auto original_hl = ram->read(combineRegisters(h, l));
        auto hl = original_hl;
        hl = hl >> 1;
        setC(original_hl & 0b00000001);
        setZ(l == 0);
        setH(0);
        setN(0);
        ram->write8(combineRegisters(h, l), hl);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x3F:
        // srl a
    {
        auto original_a = a;
        a = a >> 1;
        setC(original_a & 0b00000001);
        setZ(a == 0);
        setH(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
    }
    break;

    // SWAP
    case 0x30:
        // swap B
        b = swap8(b);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x31:
        // swap C
        c = swap8(c);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x32:
        // swap D
        d = swap8(d);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x33:
        // swap E
        e = swap8(e);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x34:
        // swap H
        h = swap8(h);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x35:
        // swap L
        l = swap8(l);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
    case 0x36:
        // swap (hl)
    {
        auto hl = combineRegisters(h, l);
        ram->write8(hl, swap8(ram->read(hl)));
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 16;
    }
    break;
    case 0x37:
        // swap A
        a = swap8(a);
        setC(0);
        setH(0);
        setZ(0);
        setN(0);
        PC++;
        lastClockCycle = 8;
        break;
        // BIT
    case 0x40:
        // bit 0, b
        testBitInstruction(0, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x41:
        // bit 0, c
        testBitInstruction(0, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x42:
        // bit 0, d
        testBitInstruction(0, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x43:
        // bit 0, e
        testBitInstruction(0, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x44:
        // bit 0, h
        testBitInstruction(0, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x45:
        // bit 0, l
        testBitInstruction(0, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x46:
        // bit 0, (hl)
        testBitInstruction(0, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x47:
        // bit 0, a
        testBitInstruction(0, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x48:
        // bit 1, b
        testBitInstruction(1, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x49:
        // bit 1, c
        testBitInstruction(1, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x4A:
        // bit 1, d
        testBitInstruction(1, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x4B:
        // bit 1, e
        testBitInstruction(1, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x4C:
        // bit 1, h
        testBitInstruction(1, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x4D:
        // bit 1, l
        testBitInstruction(1, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x4E:
        // bit 1, (hl)
        testBitInstruction(1, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x4F:
        // bit 1, a
        testBitInstruction(1, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x50:
        // bit 2, b
        testBitInstruction(2, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x51:
        // bit 2, c
        testBitInstruction(2, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x52:
        // bit 2, d
        testBitInstruction(2, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x53:
        // bit 2, e
        testBitInstruction(2, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x54:
        // bit 2, h
        testBitInstruction(2, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x55:
        // bit 2, l
        testBitInstruction(2, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x56:
        // bit 2, (hl)
        testBitInstruction(2, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x57:
        // bit 2, a
        testBitInstruction(2, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x58:
        // bit 3, b
        testBitInstruction(3, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x59:
        // bit 3, c
        testBitInstruction(3, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x5A:
        // bit 3, d
        testBitInstruction(3, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x5B:
        // bit 3, e
        testBitInstruction(3, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x5C:
        // bit 3, h
        testBitInstruction(3, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x5D:
        // bit 3, l
        testBitInstruction(3, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x5E:
        // bit 3, (hl)
        testBitInstruction(3, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x5F:
        // bit 3, a
        testBitInstruction(3, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x60:
        // bit 4, b
        testBitInstruction(4, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x61:
        // bit 4, c
        testBitInstruction(4, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x62:
        // bit 4, d
        testBitInstruction(4, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x63:
        // bit 4, e
        testBitInstruction(4, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x64:
        // bit 4, h
        testBitInstruction(4, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x65:
        // bit 4, l
        testBitInstruction(4, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x66:
        // bit 4, (hl)
        testBitInstruction(4, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x67:
        // bit 4, a
        testBitInstruction(4, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x68:
        // bit 5, b
        testBitInstruction(5, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x69:
        // bit 5, c
        testBitInstruction(5, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x6A:
        // bit 5, d
        testBitInstruction(5, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x6B:
        // bit 5, e
        testBitInstruction(5, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x6C:
        // bit 5, h
        testBitInstruction(5, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x6D:
        // bit 5, l
        testBitInstruction(5, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x6E:
        // bit 5, (hl)
        testBitInstruction(5, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x6F:
        // bit 5, a
        testBitInstruction(5, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x70:
        // bit 6, b
        testBitInstruction(6, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x71:
        // bit 6, c
        testBitInstruction(6, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x72:
        // bit 6, d
        testBitInstruction(6, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x73:
        // bit 6, e
        testBitInstruction(6, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x74:
        // bit 6, h
        testBitInstruction(6, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x75:
        // bit 6, l
        testBitInstruction(6, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x76:
        // bit 6, (hl)
        testBitInstruction(6, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x77:
        // bit 6, a
        testBitInstruction(6, a);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x78:
        // bit 7, b
        testBitInstruction(7, b);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x79:
        // bit 7, c
        testBitInstruction(7, c);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x7A:
        // bit 7, d
        testBitInstruction(7, d);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x7B:
        // bit 7, e
        testBitInstruction(7, e);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x7C:
        // bit 7, h
        testBitInstruction(7, h);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x7D:
        // bit 7, l
        testBitInstruction(7, l);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x7E:
        // bit 7, (hl)
        testBitInstruction(7, ram->read(combineRegisters(h, l)));
        lastClockCycle = 12;
        PC++;
        break;
    case 0x7F:
        // bit 7, a
        testBitInstruction(7, a);
        lastClockCycle = 8;
        PC++;
        break;

        // RESET BITS
    case 0x80:
        // reset 0, b
        modifyBit(b, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x81:
        // reset 0, c
        modifyBit(c, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x82:
        // reset 0, d
        modifyBit(d, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x83:
        // reset 0, e
        modifyBit(e, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x84:
        // reset 0, h
        modifyBit(h, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x85:
        // reset 0, l
        modifyBit(l, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x86:
        // reset 0, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 0, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0x87:
        // reset 0, a
        modifyBit(a, 0, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x88:
        // reset 1, b
        modifyBit(b, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x89:
        // reset 1, c
        modifyBit(c, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x8A:
        // reset 1, d
        modifyBit(d, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x8B:
        // reset 1, e
        modifyBit(e, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x8C:
        // reset 1, h
        modifyBit(h, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x8D:
        // reset 1, l
        modifyBit(l, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x8E:
        // reset 1, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 1, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0x8F:
        // reset 1, a
        modifyBit(a, 1, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x90:
        // reset 2, b
        modifyBit(b, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x91:
        // reset 2, c
        modifyBit(c, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x92:
        // reset 2, d
        modifyBit(d, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x93:
        // reset 2, e
        modifyBit(e, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x94:
        // reset 2, h
        modifyBit(h, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x95:
        // reset 2, l
        modifyBit(l, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x96:
        // reset 2, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 2, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0x97:
        // reset 2, a
        modifyBit(a, 2, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x98:
        // reset 3, b
        modifyBit(b, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x99:
        // reset 3, c
        modifyBit(c, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x9A:
        // reset 3, d
        modifyBit(d, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x9B:
        // reset 3, e
        modifyBit(e, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x9C:
        // reset 3, h
        modifyBit(h, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x9D:
        // reset 3, l
        modifyBit(l, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0x9E:
        // reset 3, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 3, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0x9F:
        // reset 3, a
        modifyBit(a, 3, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA0:
        // reset 4, b
        modifyBit(b, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA1:
        // reset 4, c
        modifyBit(c, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA2:
        // reset 4, d
        modifyBit(d, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA3:
        // reset 4, e
        modifyBit(e, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA4:
        // reset 4, h
        modifyBit(h, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA5:
        // reset 4, l
        modifyBit(l, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA6:
        // reset 4, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 4, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xA7:
        // reset 4, a
        modifyBit(a, 4, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA8:
        // reset 5, b
        modifyBit(b, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xA9:
        // reset 5, c
        modifyBit(c, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xAA:
        // reset 5, d
        modifyBit(d, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xAB:
        // reset 5, e
        modifyBit(e, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xAC:
        // reset 5, h
        modifyBit(h, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xAD:
        // reset 5, l
        modifyBit(l, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xAE:
        // reset 5, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 5, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xAF:
        // reset 5, a
        modifyBit(a, 5, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB0:
        // reset 6, b
        modifyBit(b, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB1:
        // reset 6, c
        modifyBit(c, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB2:
        // reset 6, d
        modifyBit(d, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB3:
        // reset 6, e
        modifyBit(e, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB4:
        // reset 6, h
        modifyBit(h, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB5:
        // reset 6, l
        modifyBit(l, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB6:
        // reset 6, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 6, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xB7:
        // reset 6, a
        modifyBit(a, 6, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB8:
        // reset 7, b
        modifyBit(b, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xB9:
        // reset 7, c
        modifyBit(c, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xBA:
        // reset 7, d
        modifyBit(d, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xBB:
        // reset 7, e
        modifyBit(e, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xBC:
        // reset 7, h
        modifyBit(h, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xBD:
        // reset 7, l
        modifyBit(l, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xBE:
        // reset 7, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 7, 0);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xBF:
        // reset 7, a
        modifyBit(a, 7, 0);
        lastClockCycle = 8;
        PC++;
        break;

        // SET BITS
    case 0xC0:
        // set 0, b
        modifyBit(b, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC1:
        // set 0, c
        modifyBit(c, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC2:
        // set 0, d
        modifyBit(d, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC3:
        // set 0, e
        modifyBit(e, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC4:
        // set 0, h
        modifyBit(h, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC5:
        // set 0, l
        modifyBit(l, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC6:
        // set 0, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 0, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xC7:
        // set 0, a
        modifyBit(a, 0, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC8:
        // set 1, b
        modifyBit(b, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xC9:
        // set 1, c
        modifyBit(c, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xCA:
        // set 1, d
        modifyBit(d, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xCB:
        // set 1, e
        modifyBit(e, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xCC:
        // set 1, h
        modifyBit(h, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xCD:
        // set 1, l
        modifyBit(l, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xCE:
        // set 1, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 1, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xCF:
        // set 1, a
        modifyBit(a, 1, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD0:
        // set 2, b
        modifyBit(b, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD1:
        // set 2, c
        modifyBit(c, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD2:
        // set 2, d
        modifyBit(d, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD3:
        // set 2, e
        modifyBit(e, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD4:
        // set 2, h
        modifyBit(h, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD5:
        // set 2, l
        modifyBit(l, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD6:
        // set 2, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 2, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xD7:
        // set 2, a
        modifyBit(a, 2, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD8:
        // set 3, b
        modifyBit(b, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xD9:
        // set 3, c
        modifyBit(c, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xDA:
        // set 3, d
        modifyBit(d, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xDB:
        // set 3, e
        modifyBit(e, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xDC:
        // set 3, h
        modifyBit(h, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xDD:
        // set 3, l
        modifyBit(l, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xDE:
        // set 3, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 3, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xDF:
        // set 3, a
        modifyBit(a, 3, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE0:
        // set 4, b
        modifyBit(b, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE1:
        // set 4, c
        modifyBit(c, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE2:
        // set 4, d
        modifyBit(d, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE3:
        // set 4, e
        modifyBit(e, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE4:
        // set 4, h
        modifyBit(h, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE5:
        // set 4, l
        modifyBit(l, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE6:
        // set 4, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 4, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xE7:
        // set 4, a
        modifyBit(a, 4, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE8:
        // set 5, b
        modifyBit(b, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xE9:
        // set 5, c
        modifyBit(c, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xEA:
        // set 5, d
        modifyBit(d, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xEB:
        // set 5, e
        modifyBit(e, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xEC:
        // set 5, h
        modifyBit(h, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xED:
        // set 5, l
        modifyBit(l, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xEE:
        // set 5, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 5, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xEF:
        // set 5, a
        modifyBit(a, 5, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF0:
        // set 6, b
        modifyBit(b, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF1:
        // set 6, c
        modifyBit(c, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF2:
        // set 6, d
        modifyBit(d, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF3:
        // set 6, e
        modifyBit(e, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF4:
        // set 6, h
        modifyBit(h, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF5:
        // set 6, l
        modifyBit(l, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF6:
        // set 6, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 6, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xF7:
        // set 6, a
        modifyBit(a, 6, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF8:
        // set 7, b
        modifyBit(b, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xF9:
        // set 7, c
        modifyBit(c, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xFA:
        // set 7, d
        modifyBit(d, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xFB:
        // set 7, e
        modifyBit(e, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xFC:
        // set 7, h
        modifyBit(h, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xFD:
        // set 7, l
        modifyBit(l, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;
    case 0xFE:
        // set 7, (hl)
        modifyBit(ram->read(combineRegisters(h, l)), 7, 1);
        lastClockCycle = 16;
        PC++;
        break;
    case 0xFF:
        // set 7, a
        modifyBit(a, 7, 1);
        lastClockCycle = 8;
        PC++;
        break;

    default:
        if (!isOnIllegalInstruction)
            printf("Not implemented CB %x - PC %x (extension)\n", opcode, PC);

        isOnIllegalInstruction = true;
        break;
    }


}

uint8_t CPU::swap8(uint8_t reg)
{
    uint8_t tmp = reg;
    reg = reg << 4;
    reg = reg | (tmp >> 4);

    return reg;
}

void CPU::testBitInstruction(uint8_t pos, uint8_t reg)
{
    setN(false);
    setH(true);
    setZ(((1 << pos) & reg) != (1 << pos));
}
