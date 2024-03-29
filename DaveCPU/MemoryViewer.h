#pragma once
#include "olcConsoleGameEngineOOP.h"
#include "CPU.h"
#include "Terminal.h"

namespace DaveCPU {

	class MemoryWindow {
	private:
		uint16_t beginAddress = 0;
		uint16_t endAddress = 0;

	public:
		int dwordsPerLine = 8;
		std::string title = "";
		MemoryWindow(uint16_t beginAddress, uint16_t endAddress, int dwordsPerLine, std::string title);

		int size() const;
		uint16_t getAddressOf(uint16_t i) const;
	};

	class MemoryViewer : public olcConsoleGameEngineOOP
	{
	private:
		CPU* attachedCPU;
        Terminal* attachedTerminal;

		double toNextInstruction = 1;
		double instructionTime = 1;

		MemoryWindow eepromWindow = MemoryWindow(0x0000, 0x004F, 8, "EEPROM");
		MemoryWindow ramWindow = MemoryWindow(0x2000, 0x204F, 8, "RAM");
        MemoryWindow vramWindow = MemoryWindow(0xA000, 0xA04F, 8, "VRAM");

	public:

		MemoryViewer();

		void attachCPU(DaveCPU::CPU* cpu);
        void attachTerminal(DaveCPU::Terminal* terminal);
		void drawMemoryWindow(int x, int y, const MemoryWindow& memoryWindow);
		void drawRegister(int x, int y, const std::string& registerName, uint16_t registerID);
		void drawCPUState(int x, int y);
        void drawTerminal(int x, int y);
		void setInstructionTime(double instructionTime);

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
	};

}