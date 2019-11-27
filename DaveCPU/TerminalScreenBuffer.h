#pragma once
#include "BusComponent.h"
#include <vector>

namespace DaveCPU {
    class TerminalScreenBuffer :
        public BusComponent
    {
    private:
        std::vector<uint16_t> memory;
    public:
        TerminalScreenBuffer();
        uint16_t read(uint16_t address) override;
        void write(uint16_t address, uint16_t data) override;
    };
}
