#pragma once
#include <string>
#include "RAM.h"
#include "EEPROM.h"
#include "TerminalScreenBuffer.h"

#define EEPROM_BEGIN	0x0000
#define EEPROM_END		0x1FFF
#define EEPROM_SIZE		0x2000
#define RAM_BEGIN		0x2000
#define RAM_END			0x9FFF
#define RAM_SIZE		0x8000
#define VRAM_BEGIN		0xA000
#define VRAM_SIZE		0x1000
#define VRAM_END		0xAFFF

namespace DaveCPU {
	class Bus
	{
	public:
		Bus();

		EEPROM eeprom;
		RAM ram;
        TerminalScreenBuffer screenBuffer;

		BusComponent& getMappedComponent(uint16_t address);
		uint16_t read(uint16_t address);
		void write(uint16_t address, uint16_t data);

		std::string dumpMemoryRange(uint16_t beginAddress, uint16_t endAddress, int bytesPerRow = 16);
	};

}