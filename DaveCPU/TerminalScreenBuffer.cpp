#include "TerminalScreenBuffer.h"

DaveCPU::TerminalScreenBuffer::TerminalScreenBuffer()
{
    memory = std::vector<uint16_t>(0x1000, 0x0000);
}

uint16_t DaveCPU::TerminalScreenBuffer::read(uint16_t address)
{
    auto index = address - 0xA000;
    if (index >= 0 && index < 0x1000)
        return memory[index];
}

void DaveCPU::TerminalScreenBuffer::write(uint16_t address, uint16_t data)
{
    auto index = address - 0xA000;
    if (index >= 0 && index < 0x1000)
        memory[index] = data;
}

uint16_t DaveCPU::TerminalScreenBuffer::get(uint16_t index)
{
    if (index < memory.size())
        return memory[index];
    else return 0;
}
