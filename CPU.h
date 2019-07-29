#pragma once
#include "Rom.h"

enum Flag {
    C = 4,
    H = 5,
    N = 6,
    Z = 7
};

class CPU
{
public:
    // Program Counter
    unsigned int PC;

    CPU(Rom rom);
    void ExecuteInstruction(uint8_t instruction);

    uint8_t RAM[65535] = { 0 };
private:
    // Registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
    uint16_t SP;

    uint16_t fetchNext2BytesInverted(int PC);

    bool isFlagSet(Flag flag);
    void setZ(bool value);
    void setN(bool value);
    void setH(bool value);
    void setC(bool value);
    
    uint16_t combineRegisters(uint8_t reg1, uint8_t reg2);

    void increaseRegister(uint8_t* reg);
    void increaseRegister(uint8_t* reg1, uint8_t* reg2);
    void decreaseRegister(uint8_t* reg);
    void decreaseRegister(uint8_t* reg1, uint8_t* reg2);

    void bitExtensions(uint8_t opcode);

    void compare(uint8_t value);
};
