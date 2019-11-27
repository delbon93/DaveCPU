#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Bus.h"
#include "CPU.h"
#include "MemoryViewer.h"
#include "LoadProgram.h"
#include "Terminal.h"
#include "ConsoleConfig.h"

namespace DaveCPU {

	CPU* setupCPU(const std::string& programFileName) {
		auto cpu = new CPU();
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
	memoryViewer.ConstructConsole(188, 37, CHAR_SIZE_SCALE(4), CHAR_SIZE_SCALE(8));
	memoryViewer.Start();

	return 0;
}