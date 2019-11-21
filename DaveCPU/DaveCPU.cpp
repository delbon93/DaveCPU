#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Bus.h"
#include "CPU.h"
#include "MemoryViewer.h"

namespace DaveCPU {

	CPU* setupCPU() {
		auto cpu = new CPU();

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

		std::vector<uint16_t> data{ };

		cpu->bus.eeprom.flash(program2);
		cpu->bus.ram.writeBulk(0x2000, data);

		return cpu;
	}
}

int main() {
	// Create a CPU instance and attach it to the memory viewer
	DaveCPU::CPU* cpu = DaveCPU::setupCPU();
	DaveCPU::MemoryViewer memoryViewer;
	memoryViewer.attachCPU(cpu);

	// Setup
	memoryViewer.setInstructionTime(0.6);
	memoryViewer.ConstructConsole(160, 50, 8, 16);

	// Starting the memory viewer, which in turn starts the CPU
	memoryViewer.Start();

	return 0;
}