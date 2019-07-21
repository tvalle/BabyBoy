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
private:
    Rom rom;

    // Registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;

    uint16_t fetchNext2BytesInverted(Rom rom, int PC);
    bool isFlagSet(Flag flag);
    uint16_t combineRegisters(uint8_t reg1, uint8_t reg2);
};
