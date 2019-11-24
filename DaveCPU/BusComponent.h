#pragma once
#include <cstdint>

namespace DaveCPU {

	class BusComponent
	{
	public:
		virtual ~BusComponent() = default;
		virtual uint16_t read(uint16_t address) = 0;
		virtual void write(uint16_t address, uint16_t data) = 0;
	};

}