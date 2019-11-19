#pragma once
#include "olcConsoleGameEngineOOP.h"
#include "CPU.h"

namespace DaveCPU {

	class MemoryWindow {
	private:
		uint16_t beginAddress = 0;
		uint16_t endAddress = 0;

	public:
		int dwordsPerLine = 8;
		std::string title = "";
		MemoryWindow(uint16_t beginAddress, uint16_t endAddress, int dwordsPerLine, std::string title);

		int size();
		uint16_t getAddressOf(uint16_t i);
	};

	class MemoryViewer : public olcConsoleGameEngineOOP
	{
	private:
		DaveCPU::CPU* attachedCPU;
		double toNextInstruction = 1;
		double instructionTime = 1;

	public:

		MemoryViewer();

		void attach(DaveCPU::CPU* cpu);
		void drawMemoryWindow(int x, int y, MemoryWindow& memoryWindow);
		void drawRegister(int x, int y, std::string registerName, uint16_t registerID);
		void drawCPUState(int x, int y);
		void setInstructionTime(double instructionTime);

		virtual bool OnUserCreate();
		virtual bool OnUserUpdate(float fElapsedTime);
	};

}