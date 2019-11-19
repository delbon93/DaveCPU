#include <iomanip>
#include <sstream>
#include "MemoryViewer.h"


#define _HEX_(value)		std::uppercase << std::setfill('.') << std::setw(sizeof(uint16_t) * 2) << std::hex << value
#define _HEXDIGIT_(value)	std::uppercase << std::setw(1) << std::hex << value
#define _HEXADDRESS_(value) "0x" << std::uppercase << std::setfill('0') << std::setw(sizeof(uint16_t) * 2) << std::hex << value

namespace DaveCPU {

	/* MemoryViewer */

	std::wstring convertString(std::string str) {
		std::wstring wstr;
		wstr.assign(str.begin(), str.end());
		return wstr;
	}

	MemoryViewer::MemoryViewer()
	{
		attachedCPU = nullptr;
		m_sAppName = convertString("DaveCPU");
	}

	void MemoryViewer::attach(DaveCPU::CPU* cpu)
	{
		attachedCPU = cpu;
	}

	void MemoryViewer::drawMemoryWindow(int x, int y, MemoryWindow& memoryWindow)
	{
		std::stringstream sstream;
		int currentX = x + 1, currentY = y + 1, col = 0;

		DrawString(x + 1, y, convertString(memoryWindow.title));

		for (int i = 0; i < memoryWindow.size(); i++) {
			// Create string
			uint16_t address = memoryWindow.getAddressOf(i);
			uint16_t value = attachedCPU->bus.read(address);

			if (col == 0) {
				sstream << _HEXADDRESS_(address);
				std::string addrStr = sstream.str();
				sstream = std::stringstream();
				DrawString(currentX, currentY, convertString(addrStr), COLOUR::BG_DARK_GREY + COLOUR::FG_BLACK);
				currentX += addrStr.size();
				DrawString(currentX, currentY, convertString(" | "), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
				currentX += 3;
			}

			sstream << _HEX_(value) << " ";

			// Set colors
			short cFG = COLOUR::FG_WHITE;
			short cBG = COLOUR::BG_BLACK;
			if (value == 0x0000) {
				cFG = COLOUR::FG_DARK_GREY;
				cBG = COLOUR::BG_BLACK;
			}
			if (address == attachedCPU->previousProgramCounter) {
				cBG = COLOUR::BG_DARK_GREEN;
			}

			// Draw string
			std::string str = sstream.str();
			sstream = std::stringstream();
			DrawString(currentX, currentY, convertString(str), cBG + cFG);

			// Change drawing coordinates
			currentX += str.size() + 1;
			col++;
			if (col == memoryWindow.dwordsPerLine) {
				DrawString(currentX, currentY, convertString("|"), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
				currentX = x + 1;
				currentY += 1;
				col = 0;
			}

		}

		int borderLength = 3 + memoryWindow.dwordsPerLine * 6;
		int borderOffset = 8;
		sstream << "+";
		for (int i = 1; i < borderLength - 1; i++) sstream << "-";
		sstream << "+";
		std::string horBorder = sstream.str();
		sstream = std::stringstream();
		DrawString(x + borderOffset, y, convertString(horBorder), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
		DrawString(x + borderOffset, currentY, convertString(horBorder), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
	}

	void MemoryViewer::drawRegister(int x, int y, std::string registerName, uint16_t registerID) {
		std::stringstream sstream;
		sstream << registerName << ": ";
		DrawString(x, y, convertString(sstream.str()), COLOUR::BG_BLACK + COLOUR::FG_GREEN);
		sstream = std::stringstream();

		uint16_t regValue = attachedCPU->getRegister(registerID);

		sstream << regValue << " (" << _HEX_(regValue) << ")";
		DrawString(x + 5, y, convertString(sstream.str()));
	}

	void MemoryViewer::drawCPUState(int x, int y)
	{
		int currentX = x + 1, currentY = y + 1;

		drawRegister(currentX, currentY++, " pc", 0x01);
		drawRegister(currentX, currentY++, "acc", 0x02);
		drawRegister(currentX, currentY++, " sf", 0x00);
		drawRegister(currentX, currentY++, "rel", 0x03);
		drawRegister(currentX, currentY++, "fet", 0x04);
		drawRegister(currentX, currentY++, "ins", 0x05);
		drawRegister(currentX, currentY++, " p1", 0x06);
		drawRegister(currentX, currentY++, " p2", 0x07);
		drawRegister(currentX, currentY++, "ret", 0x08);
		drawRegister(currentX, currentY++, "sfo", 0x09);
		drawRegister(currentX, currentY++, " am", 0x0A);
		currentY = y + 1;
		currentX += 20;
		for (uint16_t i = 0; i <= 15; i++) {
			std::stringstream sstream;
			sstream << " r" << _HEXDIGIT_(i);
			drawRegister(currentX, currentY++, sstream.str(), 0x10 + i);
			if (i == 7) {
				currentY = y + 1;
				currentX += 20;
			}
		}

		if (attachedCPU->stopExecution) {
			DrawString(x, y + 15, convertString("Execution stopped"), COLOUR::BG_DARK_RED + COLOUR::FG_BLACK);
		}
	}

	void MemoryViewer::setInstructionTime(double instructionTime)
	{
		this->instructionTime = instructionTime;
		toNextInstruction = instructionTime;
	}

	bool MemoryViewer::OnUserCreate()
	{
		return true;
	}

	bool MemoryViewer::OnUserUpdate(float fElapsedTime)
	{
		if (!attachedCPU->stopExecution) {
			toNextInstruction -= fElapsedTime;
			if (toNextInstruction <= 0) {
				toNextInstruction = instructionTime;
				attachedCPU->step();
			}
		}

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', COLOUR::BG_BLACK);
		MemoryWindow eepromWindow(0x0000, 0x009F, 8, "EEPROM");
		MemoryWindow ramWindow(0x2000, 0x209F, 8, "RAM");
		drawMemoryWindow(4, 2, eepromWindow);
		drawMemoryWindow(4, 24, ramWindow);
		drawCPUState(72, 2);

		return true;
	}

	/* MemoryWindow */

	MemoryWindow::MemoryWindow(uint16_t beginAddress, uint16_t endAddress, int dwordsPerLine = 8, std::string title = "")
	{
		this->beginAddress = beginAddress;
		this->endAddress = endAddress;
		this->dwordsPerLine = dwordsPerLine;
		this->title = title;
	}

	int MemoryWindow::size()
	{
		return endAddress - beginAddress + 1;
	}

	uint16_t MemoryWindow::getAddressOf(uint16_t i)
	{
		if (i >= 0 && i < size())
			return beginAddress + i;
		return 0;
	}

}