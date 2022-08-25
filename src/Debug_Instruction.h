#pragma once

#include <stdint.h>

struct DebugInstruction {
    uint16_t PC;
    uint8_t opcode;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
    uint16_t SP;

    DebugInstruction()
    {
    }

    DebugInstruction(uint16_t _pc, uint8_t _opcode, uint8_t _a, uint8_t _b, uint8_t _c, uint8_t _d, uint8_t _e, uint8_t _f, uint8_t _h, uint8_t _l, uint16_t _sp)
    {
        PC = _pc;
        opcode = _opcode;
        a = _a;
        b = _b;
        c = _c;
        d = _d;
        e = _e;
        f = _f;
        h = _h;
        l = _l;
        SP = _sp;
    }
};