#include "pch.h"
#include "CppUnitTest.h"
#include "CPU.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BabyTests
{
    //TODO: add test cases to only write in legal areas in RAM

    TEST_CLASS(CPULoad_hl_x)
    {
    public:
        TEST_METHOD(LoadRegisters_0x70)
        {
            RAM ram = RAM();

            // LD (hl), b
            CPU cpu = CPU(ram);
            cpu.h = 0xff;
            cpu.l = 0x09;

            cpu.b = 0x69;

            Assert::AreEqual<uint8_t>(ram.read(0xFF09), 0x00);

            cpu.ExecuteInstruction(0x70);

            Assert::AreEqual<uint8_t>(ram.read(0xFF09), 0x69);
        }
    };
}
