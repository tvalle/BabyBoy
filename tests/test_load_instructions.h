#pragma once

#include "doctest.h"
#include "../src/CPU.h"

TEST_CASE("LD r8, r8 instructions") {
    RAM ram = RAM();
    
    CPU cpu = CPU(ram); 

    cpu.a = 0x01;
    cpu.b = 0x02;
    cpu.c = 0x03;
    cpu.d = 0x04;
    cpu.e = 0x05;
    // TODO: verify valid values for HL which will be used on (HL) instructions
    cpu.h = 0x05;
    cpu.l = 0x05;

    cpu.f = 0b00000000;

    SUBCASE("0x40 LD B, B") {
        cpu.ExecuteInstruction(0x40);

        CHECK(cpu.b == 0x02);
        CHECK(cpu.lastClockCycle == 4);
        CHECK(cpu.f == 0b00000000);
    }

    SUBCASE("0x41 LD B, C") {
        cpu.ExecuteInstruction(0x41);

        CHECK(cpu.b == 0x03);
        CHECK(cpu.lastClockCycle == 4);
        CHECK(cpu.f == 0b00000000);
    }

    SUBCASE("0x42 LD B, D") {
        cpu.ExecuteInstruction(0x42);

        CHECK(cpu.b == 0x04);
        CHECK(cpu.lastClockCycle == 4);
        CHECK(cpu.f == 0b00000000);
    }

    SUBCASE("0x43 LD B, E") {
        cpu.ExecuteInstruction(0x43);

        CHECK(cpu.b == 0x05);
        CHECK(cpu.lastClockCycle == 4);
        CHECK(cpu.f == 0b00000000);
    }

    // TODO: verify valid values for HL which will be used on (HL) instructions
    // SUBCASE("0x44 LD B, H") {
    //     cpu.ExecuteInstruction(0x44);

    //     CHECK(cpu.b == 0x04);
    //     CHECK(cpu.lastClockCycle == 4);
    //     CHECK(cpu.f == 0b00000000);
    // }

    // SUBCASE("0x45 LD B, L") {
    //     cpu.ExecuteInstruction(0x45);

    //     CHECK(cpu.b == 0x04);
    //     CHECK(cpu.lastClockCycle == 4);
    //     CHECK(cpu.f == 0b00000000);
    // }
}
