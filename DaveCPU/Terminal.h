#pragma once
#include "TerminalScreenBuffer.h"
#include "olcConsoleGameEngineOOP.h"

namespace DaveCPU  {
    class Terminal
    {
    private:
        TerminalScreenBuffer* buffer;
        std::vector<std::wstring> lines;
        bool asciiMode = false;
    public:
        Terminal();

        void attachScreenBuffer(TerminalScreenBuffer* buffer);
        void Update(float fElapsedTime);
        uint16_t bufferGet(uint16_t index);
        void clear();
        void put(int x, int y, wchar_t c);
        void putw(int pos, wchar_t c);
        void puts(int x, int y, const std::wstring& str);
        void putsw(int x, int y, const std::wstring& str);
        wchar_t get(int x, int y);
        wchar_t getw(int pos);

    };
}