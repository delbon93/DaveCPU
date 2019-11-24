#include "Bus.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "CPU.h"

namespace DaveCPU {

	Bus::Bus()
	{
		eeprom = EEPROM();
		ram = RAM();
	}

	BusComponent& Bus::getMappedComponent(uint16_t address)
	{
		if (address >= EEPROM_BEGIN && address <= EEPROM_END) { // EEPROM
			return eeprom;
		}
		else {// if (address >= RAM_BEGIN && address <= RAM_END) { // RAM
			return ram;
		}/*
		else if (address >= VRAM_BEGIN && address <= VRAM_END) { // Screen Buffer
			return 0;
		}*/
	}

	uint16_t Bus::read(uint16_t address)
	{
		return getMappedComponent(address).read(address);
	}

	void Bus::write(uint16_t address, uint16_t data)
	{
		getMappedComponent(address).write(address, data);
	}

	std::string Bus::dumpMemoryRange(uint16_t beginAddress, uint16_t endAddress, int bytesPerRow)
	{
#define _HEX_(value) std::setfill('.') << std::setw(sizeof(uint16_t) * 2) << std::hex << value

		std::stringstream stream;
		stream << std::uppercase << _HEX_(beginAddress) << " | ";
		int rowLength = 0;

		for (auto addr = 0; addr <= endAddress - beginAddress; addr++, rowLength++) {
			if (rowLength == bytesPerRow) {
				rowLength = 0;
				stream << "|" << std::endl << _HEX_(beginAddress + addr) << " | ";
			}
			stream << _HEX_(read(addr + beginAddress)) << " ";
		}
		stream << "|";
		return stream.str();
	}

}