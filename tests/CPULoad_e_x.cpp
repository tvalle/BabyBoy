#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_e_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x58)
        {
            RAM ram = RAM();

            // LD e, b
            CPU cpu = CPU(ram);
            cpu.e = 0x01;
            cpu.b = 0xff;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x58);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x59)
        {
            RAM ram = RAM();

            // LD e, c
            CPU cpu = CPU(ram);
            cpu.e = 0x01;
            cpu.c = 0xff;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x59);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x5A)
        {
            RAM ram = RAM();

            // LD e, d
            CPU cpu = CPU(ram);
            cpu.e = 0x01;
            cpu.d = 0xff;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x5A);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x5B)
        {
            RAM ram = RAM();

            // LD e, e
            CPU cpu = CPU(ram);
            cpu.e = 0x01;

            cpu.ExecuteInstruction(0x5B);

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);
        }

        TEST_METHOD(LoadRegisters_0x5C)
        {
            RAM ram = RAM();

            // LD e, h
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.e = 0x01;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x5C);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x5D)
        {
            RAM ram = RAM();

            // LD e, l
            CPU cpu = CPU(ram);
            cpu.l = 0xff;
            cpu.e = 0x01;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x5D);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x5E)
        {
            RAM ram = RAM();

            // LD e, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x45;
            cpu.l = 0xff;

            ram.write8(0x45ff, 0xff);

            cpu.e = 0x01;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x5E);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x5F)
        {
            RAM ram = RAM();

            // LD e, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.e = 0x01;

            Assert::AreEqual<uint8_t>(cpu.e, 0x01);

            cpu.ExecuteInstruction(0x5F);

            Assert::AreEqual<uint8_t>(cpu.e, 0xff);
        }
    };
}
