#include "Terminal.h"

void DaveCPU::Terminal::attachScreenBuffer(TerminalScreenBuffer* buffer)
{
    this->buffer = buffer;
}

void DaveCPU::Terminal::Update(float fElapsedTime)
{
}

uint16_t DaveCPU::Terminal::get(uint16_t index)
{
    return buffer->get(index);
}

