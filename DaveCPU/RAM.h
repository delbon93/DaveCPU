#pragma once
#include <vector>
#include "BusComponent.h"

namespace DaveCPU {

	class RAM : public BusComponent
	{
	private:
		std::vector<uint16_t> memory;
	public:

		RAM();
		/* RAM address range is 0x2000 to 0x9FFF*/
		uint16_t read(uint16_t address) override;
		void write(uint16_t address, uint16_t data) override;
		void writeBulk(uint16_t address, std::vector<uint16_t>& data);
	};

}