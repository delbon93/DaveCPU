#include <iomanip>
#include <sstream>
#include "MemoryViewer.h"

// Converts a value to a 4 wide hex value string
#define _HEX_(value)		std::uppercase << std::setfill('.') << std::setw(sizeof(uint16_t) * 2) << std::hex << value
// Converts a value to a 1 wide hex digit string
#define _HEXDIGIT_(value)	std::uppercase << std::setw(1) << std::hex << value
// Converts a value to a 4 wide hex digit address string
#define _HEXADDRESS_(value) "0x" << std::uppercase << std::setfill('0') << std::setw(sizeof(uint16_t) * 2) << std::hex << value

namespace DaveCPU {

	/* MemoryViewer */

	/*
	 * Helper function to convert strings to wide strings.
	 * @param str String to convert
	 */
	std::wstring convertString(std::string str) {
		std::wstring wstring;
		wstring.assign(str.begin(), str.end());
		return wstring;
	}

	MemoryViewer::MemoryViewer()
	{
		attachedCPU = nullptr;
		m_sAppName = convertString("DaveCPU");
	}

	/*
	 * @param cpu CPU object to attach.
	 */
	void MemoryViewer::attachCPU(DaveCPU::CPU* cpu)
	{
		attachedCPU = cpu;
	}

	/*
	 * Displays a memory window.
	 * @param x x-Coordinate of the window
	 * @param y y-Coordinate of the window
	 * @param memoryWindow Memory window to display
	 */
	void MemoryViewer::drawMemoryWindow(const int x, const int y, const MemoryWindow& memoryWindow)
	{
		std::stringstream sstream;
		auto currentX = x + 1, currentY = y + 1, col = 0;

		// Show Name
		DrawString(x + 1, y, convertString(memoryWindow.title));

		for (auto i = 0; i < memoryWindow.size(); i++) {
			// Get address and read value from bus
			auto address = memoryWindow.getAddressOf(i);
			auto value = attachedCPU->bus.read(address);

			// If at beginning of line -> show address of first dword in line
			if (col == 0) {
				sstream << _HEXADDRESS_(address);
				auto addrStr = sstream.str();
				sstream = std::stringstream();
				DrawString(currentX, currentY, convertString(addrStr), BG_DARK_GREY + FG_BLACK);
				currentX += addrStr.size();
				DrawString(currentX, currentY, convertString(" | "), BG_BLACK + FG_DARK_CYAN);
				currentX += 3;
			}

			// Value at memory address
			sstream << _HEX_(value) << " ";

			// Set colors
			short cFG = FG_WHITE;
			short cBG = BG_BLACK;
			
			// Color scheme for zero-valued memory data
			if (value == 0x0000) {
				cFG = FG_DARK_GREY;
				cBG = BG_BLACK;
			}
			// Color scheme for current instruction
			if (address == attachedCPU->previousProgramCounter - attachedCPU->fetchingParameter) {
				cBG = BG_DARK_GREEN;
			}
			// Color scheme for recently fetched parameter
			if (attachedCPU->fetchingParameter > 0 && address == attachedCPU->previousProgramCounter)
			{
				cBG = BG_DARK_YELLOW;
			}
			
			// Draw string
			auto str = sstream.str();
			sstream = std::stringstream();
			DrawString(currentX, currentY, convertString(str), cFG + cBG);

			// Advance drawing coordinates
			currentX += str.size() + 1;
			col++;
			if (col == memoryWindow.dwordsPerLine) {
				// End of line delimiter
				DrawString(currentX, currentY, convertString("|"), BG_BLACK + FG_DARK_CYAN);
				currentX = x + 1;
				currentY += 1;
				col = 0;
			}
		}

		// Show borders of entire window
		auto borderLength = 3 + memoryWindow.dwordsPerLine * 6;
		auto borderOffset = 8;
		sstream << "+";
		for (auto i = 1; i < borderLength - 1; i++) 
			sstream << "-";
		sstream << "+";
		auto horBorder = sstream.str();
		DrawString(x + borderOffset, y, convertString(horBorder), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
		DrawString(x + borderOffset, currentY, convertString(horBorder), COLOUR::BG_BLACK + COLOUR::FG_DARK_CYAN);
	}

	/*
	 * Helper function to drawCPUState
	 * Displays a single register value inside a CPU state window.
	 * @param x x-Coordinate of the displayed register values
	 * @param y y-Coordinate of the displayed register values
	 * @param registerName Displayed name of the register
	 * @param registerID CPU-internal id of the register to query its value
	 */
	void MemoryViewer::drawRegister(const int x, const int y, const std::string& registerName, const uint16_t registerID) {
		// Name of register
		const auto displayedName = registerName + ": ";
		DrawString(x, y, convertString(displayedName), COLOUR::BG_BLACK + COLOUR::FG_GREEN);

		// Hex value of register
		std::stringstream sstream;
		const auto regValue = attachedCPU->getRegister(registerID);
		sstream << regValue << " (" << _HEX_(regValue) << ")";
		DrawString(x + 5, y, convertString(sstream.str()));
	}

	/*
	 * Displays the internal state of the attached CPU object.
	 * @param x x-Coordinate of the displayed state window
	 * @param y y-Coordinate of the displayed state window
	 */
	void MemoryViewer::drawCPUState(const int x, const int y)
	{
		auto currentX = x + 1, currentY = y + 1;
		const auto header = 2;

		// Last action the CPU performed
		DrawString(currentX, currentY, convertString(attachedCPU->lastAction));

		// Passed cycles
		auto sstream = std::stringstream();
		sstream << "Cycles: " << attachedCPU->cycles;
		DrawString(currentX + 20, currentY, convertString(sstream.str()));
		currentY += header;
		
		// All internal register values
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
		currentY = y + 1 + header;
		currentX += 20;

		// Data registers
		for (uint16_t i = 0; i <= 15; i++) {
			sstream = std::stringstream();
			sstream << " r" << _HEXDIGIT_(i);
			drawRegister(currentX, currentY++, sstream.str(), 0x10 + i);
			if (i == 7) {
				currentY = y + 1 + header;
				currentX += 20;
			}
		}

		if (attachedCPU->stopExecution) {
			DrawString(x, y + 15, convertString("Execution stopped"), COLOUR::BG_DARK_RED + COLOUR::FG_BLACK);
		}
	}

	/*
	 * Sets the time each instruction takes, in seconds.
	 */
	void MemoryViewer::setInstructionTime(const double instructionTime)
	{
		this->instructionTime = instructionTime;
		toNextInstruction = instructionTime;
	}

	/*
	 * Override from olcConsoleGameEngineOOP
	 * Notifies the engine that the memory viewer has been created successfully.
	 */
	bool MemoryViewer::OnUserCreate() { return true; }

	/*
	 * Override from olcConsoleGameEngineOOP
	 * Is called every frame to update the memory viewer.
	 * @param fElapsedTime Passed time in seconds since last OnUserUpdate call
	 */
	bool MemoryViewer::OnUserUpdate(const float fElapsedTime)
	{
		// Timing for when next action takes place
		if (!attachedCPU->stopExecution) {
			toNextInstruction -= fElapsedTime;
			if (toNextInstruction <= 0) {
				toNextInstruction = instructionTime;
				attachedCPU->clockCycle();
			}
		}

		// Draw everything
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', COLOUR::BG_BLACK);
		drawMemoryWindow(4, 2, eepromWindow);
		drawMemoryWindow(4, 24, ramWindow);
		drawCPUState(72, 2);

		return true;
	}

	/* MemoryWindow */

	/*
	 * Creates a new memory window to peek into a specific part
	 * of mapped memory.
	 * @param beginAddress Start of the window
	 * @param endAddress End of the window
	 * @param dwordsPerLine Displayed number of data words per line
	 * @param title Displayed name of the window
	 */
	MemoryWindow::MemoryWindow(const uint16_t beginAddress, const uint16_t endAddress, const int dwordsPerLine = 8, 
		const std::string title = "")
	{
		this->beginAddress = beginAddress;
		this->endAddress = endAddress;
		this->dwordsPerLine = dwordsPerLine;
		this->title = title;
	}

	/*
	 * The size of the memory region this memory window covers.
	 */
	int MemoryWindow::size() const
	{
		return endAddress - beginAddress + 1;
	}

	/*
	 * The absolute bus address of an index of this memory window.
	 * @param i Index, counted relative from the begin address of the window
	 */
	uint16_t MemoryWindow::getAddressOf(const uint16_t i) const
	{
		if (i >= 0 && i < size())
			return beginAddress + i;
		return 0;
	}

}