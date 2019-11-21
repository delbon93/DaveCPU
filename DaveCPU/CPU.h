#pragma once
#include <cstdint>
#include <queue>
#include <string>
#include "Bus.h"


namespace DaveCPU {

	class CPU
	{
	public:
		Bus bus;
		unsigned long int cycles = 0;
		bool stopExecution = false;
		uint16_t previousProgramCounter = 0;
		int fetchingParameter = 0;

		std::string lastAction = "";
		std::queue<void(CPU::*)()> actionQueue;
		

		/* Registers */
		uint16_t rStatusFlags = 0;		// status flags
		uint16_t rProgramCounter = 0;	// program counter
		uint16_t rAccumulator = 0;		// accumulator
		uint16_t rRelativeAddress = 0;	// relative address
		uint16_t rFetch = 0;			// fetch
		uint16_t rInstruction = 0;		// instruction word
		uint16_t rParameter1 = 0;		// first parameter
		uint16_t rParameter2 = 0;		// second parameter
		uint16_t rAddressMode = 0;		// address modes
		uint16_t rReturnAddress = 0;	// return address
		uint16_t rStackFrameOffset = 0;	// stack frame offset
		uint16_t rDataRegisters[16];	// data registers

		CPU();

		/* Bus interaction */
		uint16_t readAbsolute(uint16_t address);
		void writeAbsolute(uint16_t address, uint16_t data);
		uint16_t readRelative(uint16_t address);
		void writeRelative(uint16_t address, uint16_t data);

		/* FD-Cycle */
		void fetch();
		void decode();
		void execute();

		void fetchParameter1();
		void fetchParameter2();

		void clockCycle();
		void run();

		/* Helper Functions */

		uint8_t getAddressMode(uint8_t param) const;
		uint16_t addAndSetCarry(int a, int b);

		uint16_t getRegister(uint16_t registerCode);
		void setRegister(uint16_t registerCode, uint16_t data);
		void jump();

		/* Status Flag Query */
		bool isZeroSet() const;
		bool isSignSet() const;
		bool isOverflowSet() const;
		bool isCarrySet() const;

		/* Opcodes */

#pragma region Opcodes
#define opLTA 0x01
#define opLTR 0x02
#define opSTM 0x03
#define opSTR 0x04
#define opADD 0x10
#define opADC 0x11
#define opSUB 0x12
#define opMUL 0x13
#define opDIV 0x14
#define opMOD 0x15
#define opINC 0x16
#define opDEC 0x17
#define opCMP 0x20
#define opAND 0x30
#define opORR 0x31
#define opNOT 0x32
#define opXOR 0x33
#define opNND 0x34
#define opNOR 0x35
#define opROL 0x36
#define opROR 0x37
#define opJMP 0x40
#define opJEQ 0x41
#define opJNE 0x42
#define opJGT 0x43
#define opJGE 0x44
#define opJLT 0x45
#define opJLE 0x46
#define opREL 0xF0
#define opCSR 0xF1
#define opRTN 0xFF
#define opSKP 0x00
#pragma endregion


	/* Instructions */
		void LTA(); void LTR(); void STM(); void STR();
		void ADD(); void ADC(); void SUB(); void MUL(); void DIV(); void MOD(); void INC(); void DEC();
		void CMP();
		void AND(); void ORR(); void NOT(); void XOR(); void NND(); void NOR(); void ROL(); void ROR();
		void JMP(); void JEQ(); void JNE(); void JGT(); void JGE(); void JLT(); void JLE();
		void REL(); void CSR(); void RTN(); void SKP();
	};

}