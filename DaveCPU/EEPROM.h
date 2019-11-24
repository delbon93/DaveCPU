#pragma once
#include <vector>
#include "BusComponent.h"

namespace DaveCPU {

	class EEPROM : public BusComponent
	{
	private:
		std::vector<uint16_t> memory;
	public:
		EEPROM();

		uint16_t read(uint16_t address);
		void write(uint16_t address, uint16_t data);
		void flash(std::vector<uint16_t>& image);
	};

}