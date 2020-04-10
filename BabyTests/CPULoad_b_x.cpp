#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    TEST_CLASS(CPULoad_b_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x40)
        {
            RAM ram = RAM();

            // LD b, b
            CPU cpu = CPU(ram);
            cpu.b = 0xff;

            cpu.ExecuteInstruction(0x40);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x41)
        {
            RAM ram = RAM();

            // LD b, c
            CPU cpu = CPU(ram);
            cpu.c = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x41);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x42)
        {
            RAM ram = RAM();

            // LD b, d
            CPU cpu = CPU(ram);
            cpu.d = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x42);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x43)
        {
            RAM ram = RAM();

            // LD b, e
            CPU cpu = CPU(ram);
            cpu.e = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x43);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x44)
        {
            RAM ram = RAM();

            // LD b, h
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x44);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x45)
        {
            RAM ram = RAM();

            // LD b, l
            CPU cpu = CPU(ram);
            cpu.l = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x45);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x46)
        {
            RAM ram = RAM();

            // LD b, (hl)
            CPU cpu = CPU(ram);
            cpu.h = 0x00;
            cpu.l = 0x00;
            
            ram.write8(0x0000, 0xff);

            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x46);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }

        TEST_METHOD(LoadRegisters_0x47)
        {
            RAM ram = RAM();

            // LD b, a
            CPU cpu = CPU(ram);
            cpu.a = 0xff;
            cpu.b = 0x01;

            Assert::AreEqual<uint8_t>(cpu.b, 0x01);

            cpu.ExecuteInstruction(0x47);

            Assert::AreEqual<uint8_t>(cpu.b, 0xff);
        }
    };
}
