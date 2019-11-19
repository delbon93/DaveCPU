#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Bus.h"
#include "CPU.h"
#include "MemoryViewer.h"

namespace DaveCPU {

	CPU* setupCPU() {
		CPU* cpu = new CPU();

		/* Reads values '0x0F00' and '0x010A' from RAM (at
		 * addresses $2000 and $2001) and adds them together.
		 * Then stores the result in RAM at address $2002.
		 */
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

		std::vector<uint16_t> data { };

		cpu->bus.eeprom.flash(program2);
		cpu->bus.ram.writeBulk(0x2000, data);
		/*
		std::cout << "\t< EEPROM >" << std::endl << cpu.bus.dumpMemoryRange(0x0000, 0x0017, 8) << std::endl << std::endl;
		std::cout << "\t< RAM >" << std::endl << cpu.bus.dumpMemoryRange(0x2000, 0x2017, 8) << std::endl << std::endl;
		*/

		return cpu;
	}
}

int main() {
	DaveCPU::CPU* cpu = DaveCPU::setupCPU();
	DaveCPU::MemoryViewer memoryViewer;
	memoryViewer.attach(cpu);
	memoryViewer.setInstructionTime(0.5);
	memoryViewer.ConstructConsole(160, 50, 8, 16);
	memoryViewer.Start();

	return 0;
}