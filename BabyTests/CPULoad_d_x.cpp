#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_d_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x50)
        {
            RAM ram = RAM();

            // LD d, b
            CPU cpu = CPU(ram);
            cpu.d = 0x01;
            cpu.b = 0xff;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x50);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x51)
        {
            RAM ram = RAM();

            // LD d, c
            CPU cpu = CPU(ram);
            cpu.d = 0x01;
            cpu.c = 0xff;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x51);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x52)
        {
            RAM ram = RAM();

            // LD d, d
            CPU cpu = CPU(ram);
            cpu.d = 0xff;

            cpu.ExecuteInstruction(0x52);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x53)
        {
            RAM ram = RAM();

            // LD d, e
            CPU cpu = CPU(ram);
            cpu.e = 0xff;
            cpu.d = 0x01;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x53);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x54)
        {
            RAM ram = RAM();

            // LD d, h
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.d = 0x01;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x54);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x55)
        {
            RAM ram = RAM();

            // LD d, l
            CPU cpu = CPU(ram);
            cpu.l = 0xff;
            cpu.d = 0x01;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x55);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x56)
        {
            RAM ram = RAM();

            // LD d, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x45;
            cpu.l = 0xff;

            ram.write8(0x45ff, 0xff);

            cpu.d = 0x01;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x56);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x57)
        {
            RAM ram = RAM();

            // LD d, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.d = 0x01;

            Assert::AreEqual<uint8_t>(cpu.d, 0x01);

            cpu.ExecuteInstruction(0x57);

            Assert::AreEqual<uint8_t>(cpu.d, 0xff);
        }
    };
}
