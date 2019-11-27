#pragma once
#include "TerminalScreenBuffer.h"
#include "olcConsoleGameEngineOOP.h"

namespace DaveCPU  {
    class Terminal
    {
    private:
        TerminalScreenBuffer* buffer;
    public:
        void attachScreenBuffer(TerminalScreenBuffer* buffer);
        void Update(float fElapsedTime);
        uint16_t get(uint16_t index);
        

    };
}