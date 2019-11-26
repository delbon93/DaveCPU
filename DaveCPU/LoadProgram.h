#pragma once
#include <fstream>
#include <iterator>
#include <vector>

namespace DaveCPU {

    std::vector<uint16_t> loadProgramFromFile(const std::string& fileName) {
        streampos begin, end;
        std::fstream file(fileName, std::ios::binary | std::ios::in);
        std::vector<uint16_t> binData;

        begin = file.tellg();
        file.seekg(0, std::ios::end);
        end = file.tellg();
        int fileSize = end - begin;
        binData.reserve(fileSize);

        char* buffer = new char[2];
        for (int i = 0; i < fileSize / 2; i++) {
            file.seekg(std::ios::beg + i * 2);
            file.read(buffer, 2);
            uint16_t dataWord = static_cast<uint16_t>(buffer[1]) + (static_cast<uint16_t>(buffer[0]) << 8);
            binData.push_back(dataWord);
        }
        delete[] buffer;

        file.close();
        return binData;
    }

}