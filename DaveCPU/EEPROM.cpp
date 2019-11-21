#include "EEPROM.h"

namespace DaveCPU {

	EEPROM::EEPROM()
	{
		memory = std::vector<uint16_t>(0x2000);
	}

	uint16_t EEPROM::read(const uint16_t address)
	{
		if (address < 0x2000)
			return memory[address];
		return 0;
	}

	void EEPROM::write(uint16_t address, uint16_t data) {}

	void EEPROM::flash(std::vector<uint16_t>& image)
	{
		const auto endAddress = image.size() < 0x2000 ? static_cast<int>(image.size()) : 0x2000;
		for (auto addr = 0; addr < endAddress; addr++) {
			memory[addr] = image[addr];
		}
	}

}