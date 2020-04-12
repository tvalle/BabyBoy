#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_l_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x68)
        {
            RAM ram = RAM();

            // LD l, b
            CPU cpu = CPU(ram);
            cpu.l = 0x01;
            cpu.b = 0xff;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x68);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x69)
        {
            RAM ram = RAM();

            // LD l, c
            CPU cpu = CPU(ram);
            cpu.l = 0x01;
            cpu.c = 0xff;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x69);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x6A)
        {
            RAM ram = RAM();

            // LD l, d
            CPU cpu = CPU(ram);
            cpu.l = 0x01;
            cpu.d = 0xff;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x6A);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x6B)
        {
            RAM ram = RAM();

            // LD l, e
            CPU cpu = CPU(ram);
            cpu.l = 0x01;
            cpu.e = 0xff;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x6B);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x6C)
        {
            RAM ram = RAM();

            // LD l, h
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.l = 0x01;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x6C);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x6D)
        {
            RAM ram = RAM();

            // LD l, l
            CPU cpu = CPU(ram);
            cpu.l = 0x01;

            cpu.ExecuteInstruction(0x6D);

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);
        }

        TEST_METHOD(LoadRegisters_0x6E)
        {
            RAM ram = RAM();

            // LD l, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x45;
            cpu.l = 0xff;

            ram.write8(0x45ff, 0xAA);

            Assert::AreEqual<uint8_t>(cpu.l, 0xFF);

            cpu.ExecuteInstruction(0x6E);

            Assert::AreEqual<uint8_t>(cpu.l, 0xAA);
        }

        TEST_METHOD(LoadRegisters_0x6F)
        {
            RAM ram = RAM();

            // LD l, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.l = 0x01;

            Assert::AreEqual<uint8_t>(cpu.l, 0x01);

            cpu.ExecuteInstruction(0x6F);

            Assert::AreEqual<uint8_t>(cpu.l, 0xff);
        }
    };
}
