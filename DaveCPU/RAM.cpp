#include "RAM.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

namespace DaveCPU {

	RAM::RAM()
	{
		memory = std::vector<uint16_t>(0x8000, 0x0000);
	}

	uint16_t RAM::read(uint16_t address)
	{
		auto index = address - 0x2000;
		if (index >= 0 && index < 0x8000)
			return memory[index];
		return 0;
	}

	void RAM::write(uint16_t address, uint16_t data)
	{
		auto index = address - 0x2000;
		if (index >= 0 && index < 0x8000)
			memory[index] = data;
	}

	void RAM::writeBulk(uint16_t address, std::vector<uint16_t>& data)
	{
		for (uint16_t offset = 0; offset < data.size(); offset++)
			write(address + offset, data[offset]);
	}

}