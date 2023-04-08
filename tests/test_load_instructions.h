#pragma once

#include "doctest.h"
#include "../src/CPU.h"

TEST_CASE("LD instructions") {
    RAM ram = RAM();
    
    CPU cpu = CPU(ram); 

    cpu.a = 0x01;
    cpu.b = 0x02;
    cpu.c = 0x03;
    cpu.d = 0x04;
    cpu.e = 0x05;

    cpu.f = 0b00000000;

    SUBCASE("0x40 LD B, B") {
        cpu.ExecuteInstruction(0x40);

        CHECK(cpu.b == 0x02);
    }

    SUBCASE("0x41 LD B, C") {
        cpu.ExecuteInstruction(0x41);

        CHECK(cpu.b == 0x03);
    }
}
