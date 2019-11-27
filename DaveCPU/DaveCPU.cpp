#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Bus.h"
#include "CPU.h"
#include "MemoryViewer.h"
#include "LoadProgram.h"
#include "Terminal.h"

namespace DaveCPU {

	CPU* setupCPU(const std::string& programFileName) {
		auto cpu = new CPU();
        /*
		std::vector<uint16_t> program1{
			0xF021, 0x2000,			// REL $0x2000
			0x0161, 0x0000,			// LTA &0
			0x022E, 0x0010, 0x0001,	// LTR r0, &1
			0x1041, 0x0010,			// ADD r0
			0x0341, 0x0002,			// STM &2
			0x4021, 0x0000,			// JMP #0
			0xFF00					// RTN
		};
		std::vector<uint16_t> program2{
			0xF021, 0x2000,			// REL $0x2000
			0x0161, 0x0000,			// LTA &0
			0x1600,					// INC
			0x0341, 0x0000,			// STM &0
			0x2021, 0x0005,			// CMP 5
			0x4321, 0x0002,			// JGT 2
			0xFF00					// RTN
		};
		std::vector<uint16_t> program3{
			0xF021, 0x2000,			// REL $0x2000
			0x0121, 0x0000,			// LTA 0
			0x1600,					// INC
			0x0421, 0x001D,			// STR rD
			0x2021, 0x0005,			// CMP 5
			0x4321, 0x0004,			// JGT 3
			0x034A, 0x0000, 0x001D, // STM &0, rD
			0xFF00					// RTN
		};
        */
        std::vector<uint16_t> program = loadProgramFromFile(programFileName);
		std::vector<uint16_t> data{ 0x0005 };

		cpu->bus.eeprom.flash(program);
		cpu->bus.ram.writeBulk(0x2000, data);

		return cpu;
	}
}

int main(int argc, char* argv[]) {
    //Load the program to be flashed into EEPROM
    std::string programFileName = "../compiled_programs/default.bin";
    if (argc > 1) {
        programFileName = std::string(argv[1]);
    }

	// Create a CPU instance and attach it to the memory viewer, create a terminal window
	DaveCPU::CPU* cpu = DaveCPU::setupCPU(programFileName);
	DaveCPU::MemoryViewer memoryViewer;
    DaveCPU::Terminal terminal;

	// Setup
	memoryViewer.setInstructionTime(0.025);
    terminal.attachScreenBuffer(&(cpu->bus.screenBuffer));
	memoryViewer.attachCPU(cpu);
    memoryViewer.attachTerminal(&terminal);

	// Starting the memory viewer, which in turn starts the CPU
	memoryViewer.ConstructConsole(188, 37, 6, 12);
	memoryViewer.Start();

	return 0;
}