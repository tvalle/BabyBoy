#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_h_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x60)
        {
            RAM ram = RAM();

            // LD h, b
            CPU cpu = CPU(ram);
            cpu.h = 0x01;
            cpu.b = 0xff;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x60);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x61)
        {
            RAM ram = RAM();

            // LD h, c
            CPU cpu = CPU(ram);
            cpu.h = 0x01;
            cpu.c = 0xff;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x61);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x62)
        {
            RAM ram = RAM();

            // LD h, d
            CPU cpu = CPU(ram);
            cpu.h = 0x01;
            cpu.d = 0xff;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x62);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x63)
        {
            RAM ram = RAM();

            // LD h, e
            CPU cpu = CPU(ram);
            cpu.h = 0x01;
            cpu.e = 0xff;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x63);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x64)
        {
            RAM ram = RAM();

            // LD h, h
            CPU cpu = CPU(ram);
            cpu.h = 0x01;

            cpu.ExecuteInstruction(0x64);

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);
        }

        TEST_METHOD(LoadRegisters_0x65)
        {
            RAM ram = RAM();

            // LD h, l
            CPU cpu = CPU(ram);
            cpu.l = 0xff;
            cpu.h = 0x01;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x65);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x66)
        {
            RAM ram = RAM();

            // LD h, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x45;
            cpu.l = 0xff;

            ram.write8(0x45ff, 0xff);

            Assert::AreEqual<uint8_t>(cpu.h, 0x45);

            cpu.ExecuteInstruction(0x66);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x67)
        {
            RAM ram = RAM();

            // LD h, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.h = 0x01;

            Assert::AreEqual<uint8_t>(cpu.h, 0x01);

            cpu.ExecuteInstruction(0x67);

            Assert::AreEqual<uint8_t>(cpu.h, 0xff);
        }
    };
}
