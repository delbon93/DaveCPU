#include "Terminal.h"

DaveCPU::Terminal::Terminal()
{
    std::wstring emptyLine = L"";
    for (int i = 0; i < 80; i++) emptyLine += L' ';
    lines = std::vector<std::wstring>(24, emptyLine);
}

void DaveCPU::Terminal::attachScreenBuffer(TerminalScreenBuffer* buffer)
{
    this->buffer = buffer;
}

void DaveCPU::Terminal::Update(float fElapsedTime)
{
    for (int i = 0; i < 80*24; i++) {
        uint16_t data = buffer->get(i);
        this->putw(i, static_cast<wchar_t>(data));
    }
}

uint16_t DaveCPU::Terminal::bufferGet(uint16_t index)
{
    return buffer->get(index);
}

void DaveCPU::Terminal::clear()
{
    std::wstring emptyLine = L"";
    for (int i = 0; i < 80; i++) emptyLine += L' ';
    lines = std::vector<std::wstring>(24, emptyLine);
}

void DaveCPU::Terminal::put(int x, int y, wchar_t c)
{
    if (x >= 0 && x < 80 && y >= 0 && y < 24) {
        lines[y][x] = c;
    }
}

void DaveCPU::Terminal::putw(int pos, wchar_t c)
{
    int y = (pos / 80) % 24;
    int x = pos % 80;
    put(x, y, c);
}

void DaveCPU::Terminal::puts(int x, int y, const std::wstring& str)
{
    int _x = x;
    int _y = y;
    for (int i = 0; i < str.size() && _x < 80; i++) {
        put(_x, _y, str[i]);
        _x++;
    }
}

void DaveCPU::Terminal::putsw(int x, int y, const std::wstring& str)
{
    int _x = x;
    int _y = y;
    for (int i = 0; i < str.size(); i++) {
        put(_x, _y, str[i]);
        _x++;
        if (_x >= 80) {
            _x -= 80;
            _y++;
        }
    }
}

wchar_t DaveCPU::Terminal::get(int x, int y)
{
    if (x >= 0 && x < 80 && y >= 0 && y < 24) {
        return lines[y][x];
    }
    else return L' ';
}

wchar_t DaveCPU::Terminal::getw(int pos)
{
    int y = (pos / 80) % 24;
    int x = pos % 80;
    return get(x, y);
}

