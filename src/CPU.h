#pragma once
#include <list>
#include <vector>
#include <stdint.h>
#include "RAM.h"
#include "Debug_Instruction.h"

// This is used to save the last N instructions (for debugging)
#define MAX_INSTRUCTIONS_LIST 1000

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
    uint16_t PC;

    CPU() = default;
    CPU(RAM &ram);
    ~CPU();
    void ExecuteInstruction(uint8_t instruction);
    bool isFlagSet(Flag flag);

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
    uint16_t SP;

    int cycles;
    int lastClockCycle;

    bool isOnIllegalInstruction = false;

	std::list<DebugInstruction> lastInstructions;
private:

    // Registers
    RAM *ram;

    uint8_t modifyBit(uint8_t number, uint8_t position, uint8_t value);

    uint16_t fetchNext2BytesInverted(int PC);
    uint16_t receive2bytesFromRam(int ramPos);
	uint16_t fetchAddressFromRam(int ramPos);
    void add2bytesToRam(int ramPos, uint16_t value);

    void setZ(bool value);
    void setN(bool value);
    void setH(bool value);
    void setC(bool value);

    bool getC();
    
    uint16_t combineRegisters(uint8_t reg1, uint8_t reg2);

    void increaseMemoryAddress(uint16_t pointer);
    void decreaseMemoryAddress(uint16_t pointer);
    void increaseRegister(uint8_t* reg);
    void increaseRegister(uint8_t* reg1, uint8_t* reg2);
    void decreaseRegister(uint8_t* reg);
    void decreaseRegister(uint8_t* reg1, uint8_t* reg2);

    void addHL(uint16_t reg);

    void add(uint8_t reg);
    void adc(uint8_t reg);
    void sub(uint8_t reg);
    void sbc(uint8_t reg);
    void reg_and(uint8_t reg);
    void reg_xor(uint8_t reg);
    void reg_or(uint8_t reg);
    void reg_cp(uint8_t reg);

    void bitExtensions(uint8_t opcode);

    uint8_t swap8(uint8_t reg);
    void testBitInstruction(uint8_t pos, uint8_t reg);
};
