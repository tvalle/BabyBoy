#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_c_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x48)
        {
            RAM ram = RAM();

            // LD c, b
            CPU cpu = CPU(ram);
            cpu.c = 0x01;
            cpu.b = 0xff;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x48);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x49)
        {
            RAM ram = RAM();

            // LD c, c
            CPU cpu = CPU(ram);
            cpu.c = 0xff;

            cpu.ExecuteInstruction(0x49);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4A)
        {
            RAM ram = RAM();

            // LD c, d
            CPU cpu = CPU(ram);
            cpu.d = 0xff;
            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4A);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4B)
        {
            RAM ram = RAM();

            // LD c, e
            CPU cpu = CPU(ram);
            cpu.e = 0xff;
            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4B);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4C)
        {
            RAM ram = RAM();

            // LD c, h
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4C);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4D)
        {
            RAM ram = RAM();

            // LD c, l
            CPU cpu = CPU(ram);
            cpu.l = 0xff;
            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4D);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4E)
        {
            RAM ram = RAM();

            // LD c, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x45;
            cpu.l = 0xff;

            ram.write8(0x45ff, 0xff);

            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4E);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x4F)
        {
            RAM ram = RAM();

            // LD c, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.c = 0x01;

            Assert::AreEqual<uint8_t>(cpu.c, 0x01);

            cpu.ExecuteInstruction(0x4F);

            Assert::AreEqual<uint8_t>(cpu.c, 0xff);
        }
    };
}
