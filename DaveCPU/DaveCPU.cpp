#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Bus.h"
#include "CPU.h"
#include "MemoryViewer.h"
#include "LoadProgram.h"

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
    std::string programFileName;
    if (argc > 1) {
        programFileName = std::string(argv[1]);
    }
	// Create a CPU instance and attach it to the memory viewer
	DaveCPU::CPU* cpu = DaveCPU::setupCPU(programFileName);
	DaveCPU::MemoryViewer memoryViewer;
	memoryViewer.attachCPU(cpu);

	// Setup
	memoryViewer.setInstructionTime(0.25);
	memoryViewer.ConstructConsole(108, 25, 8, 16);

	// Starting the memory viewer, which in turn starts the CPU
	memoryViewer.Start();

	return 0;
}