#include "CPU.h"

namespace DaveCPU {

	CPU::CPU()
	{
		bus = Bus();
	}

	uint16_t CPU::readAbsolute(uint16_t address)
	{
		return bus.read(address);
	}

	void CPU::writeAbsolute(uint16_t address, uint16_t data)
	{
		bus.write(address, data);
	}

	uint16_t CPU::readRelative(uint16_t address)
	{
		return bus.read(address + rRelativeAddress);
	}

	void CPU::writeRelative(uint16_t address, uint16_t data)
	{
		bus.write(address + rRelativeAddress, data);
	}

	void CPU::fetch()
	{
		cycles++;
		rFetch = bus.read(rProgramCounter++);
	}

	void CPU::decode()
	{
		cycles++;
		// get opcode
		fetch();
		rInstruction = (rFetch >> 8) & 0x00FF;
		// get address modes
		rAddressMode = (rFetch >> 2) & 0x0007;
		rAddressMode += (rFetch << 3) & 0x0700;
		// get parameter count
		uint8_t paramCount = rFetch & 0x0003;
		if (paramCount > 0) {
			fetch();
			rParameter1 = rFetch;
			if (paramCount > 1) {
				fetch();
				rParameter2 = rFetch;
			}
		}
	}

	void CPU::execute()
	{
		cycles++;
		switch (rInstruction) {
		case opLTA: LTA(); break; case opLTR: LTR(); break; case opSTM: STM(); break; case opSTR: STR(); break;
		case opADD: ADD(); break; case opADC: ADC(); break; case opSUB: SUB(); break; case opMUL: MUL(); break;
		case opDIV: DIV(); break; case opMOD: MOD(); break; case opINC: INC(); break; case opDEC: DEC(); break;
		case opCMP: CMP(); break; case opAND: AND(); break; case opORR: ORR(); break; case opNOT: NOT(); break;
		case opXOR: XOR(); break; case opNND: NND(); break; case opNOR: NOR(); break; case opROL: ROL(); break;
		case opROR: ROR(); break; case opJMP: JMP(); break; case opJEQ: JEQ(); break; case opJNE: JNE(); break;
		case opJGT: JGT(); break; case opJGE: JGE(); break; case opJLT: JLT(); break; case opJLE: JLE(); break;
		case opREL: REL(); break; case opCSR: CSR(); break; case opRTN: RTN(); break; case opSKP: SKP(); break;
		default: SKP();
		}
	}

	void CPU::step()
	{
		previousProgramCounter = rProgramCounter;
		decode();
		execute();
	}

	void CPU::stepFor(int stepCount)
	{
		for (auto i = 0; i < stepCount; i++) step();
	}

	void CPU::run()
	{
		while (!stopExecution) step();
	}

	/* INSTRUCTIONS */

	uint8_t CPU::getAddressMode(uint8_t param)
	{
		switch (param) {
		case 0:
			return (uint8_t)(rAddressMode >> 8);
		default:
			return (uint8_t)(rAddressMode & 0x00FF);
		}
	}

	uint16_t CPU::getRegister(uint16_t registerCode)
	{
		switch (registerCode) {
		case 0: return rStatusFlags;
		case 1: return rProgramCounter;
		case 2: return rAccumulator;
		case 3: return rRelativeAddress;
		case 4: return rFetch;
		case 5: return rInstruction;
		case 6: return rParameter1;
		case 7: return rParameter2;
		case 8: return rReturnAddress;
		case 9: return rStackFrameOffset;
		case 10: return rAddressMode;
		default:
			if (registerCode >= 0x10 && registerCode <= 0x1F) return rDataRegisters[registerCode - 0x10];
			else return 0;
		}
	}

	void CPU::setRegister(uint16_t registerCode, uint16_t data)
	{
		switch (registerCode) {
		case 0: rStatusFlags = data; break;
		case 1: rProgramCounter = data; break;
		case 2: rAccumulator = data; break;
		case 3: rRelativeAddress = data; break;
		case 4: rFetch = data; break;
		case 5: rInstruction = data; break;
		case 6: rParameter1 = data; break;
		case 7: rParameter2 = data; break;
		case 8: rReturnAddress = data; break;
		case 9: rStackFrameOffset = data; break;
		case 10: rAddressMode = data; break;
		default:
			if (registerCode >= 0x10 && registerCode <= 0x1F) rDataRegisters[registerCode - 0x10] = data;
			else return;
		}
	}

	void CPU::LTA() {
		if (getAddressMode(0) == 1) rAccumulator = rParameter1;
		else if (getAddressMode(0) == 2) rAccumulator = readAbsolute(rParameter1);
		else if (getAddressMode(0) == 3) rAccumulator = readRelative(rParameter1);
		else if (getAddressMode(0) == 4) rAccumulator = getRegister(rParameter1);
	}

	void CPU::LTR() {
		uint16_t data = 0;
		if (getAddressMode(1) == 1) data = rParameter2;
		else if (getAddressMode(1) == 2) data = readAbsolute(rParameter2);
		else if (getAddressMode(1) == 3) data = readRelative(rParameter2);
		else if (getAddressMode(1) == 4) data = getRegister(rParameter2);

		if (getAddressMode(0) == 1) setRegister(rParameter1, data);
	}

	void CPU::STM() {
		uint16_t data = 0;
		if (getAddressMode(1) == 0) data = rAccumulator;
		else if (getAddressMode(1) == 1) data = rParameter2;
		else if (getAddressMode(1) == 2) data = getRegister(rParameter2);

		if (getAddressMode(0) == 1) writeAbsolute(rParameter1, data);
		else if (getAddressMode(0) == 2) writeRelative(rParameter1, data);
	}

	void CPU::STR() {
		if (getAddressMode(0) == 1) setRegister(rParameter1, rAccumulator);
	}

	uint16_t CPU::addAndSetCarry(int a, int b) {
		int uncropped = a + b;
		uint16_t result = (uint16_t)(uncropped & 0xFFFF);
		int remainder = (uncropped >> 16);
		if (remainder != 0) rStatusFlags |= 0b1100;
		return result;
	}

	void CPU::ADD() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator = addAndSetCarry(rAccumulator, data);
		else if (getAddressMode(1) == 1) rAccumulator = addAndSetCarry(data, rParameter2);
		else if (getAddressMode(1) == 2) rAccumulator = addAndSetCarry(data, getRegister(rParameter2));
	}

	void CPU::ADC() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		data += rStatusFlags & 0x0008;

		if (getAddressMode(1) == 0) rAccumulator = addAndSetCarry(rAccumulator, data);
		else if (getAddressMode(1) == 1) rAccumulator = addAndSetCarry(data, rParameter2);
		else if (getAddressMode(1) == 2) rAccumulator = addAndSetCarry(data, getRegister(rParameter2));
	}

	void CPU::SUB() {}
	void CPU::MUL() {}
	void CPU::DIV() {}
	void CPU::MOD() {}

	void CPU::INC() {
		if (getAddressMode(0) == 0) rAccumulator++;
		else if (getAddressMode(0) == 1) setRegister(rParameter1, getRegister(rParameter1) + 1);
	}

	void CPU::DEC() {
		if (getAddressMode(0) == 0) rAccumulator--;
		else if (getAddressMode(0) == 1) setRegister(rParameter1, getRegister(rParameter1) - 1);
	}

	void CPU::CMP() {
		uint16_t data = 0;

		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		uint16_t source = 0;
		if (getAddressMode(1) == 0) source = rAccumulator;
		else if (getAddressMode(1) == 1) source = rParameter2;
		else if (getAddressMode(1) == 2) source = getRegister(rParameter2);

		int cmp = source - data;
		if (cmp < 0) rStatusFlags |= 0x0001;
		else rStatusFlags &= 0xFFFE;
		if (cmp == 0) rStatusFlags |= 0x0002;
		else rStatusFlags &= 0xFFFD;
	}

	void CPU::AND() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator &= data;
		else if (getAddressMode(1) == 1) rAccumulator = data & rParameter1;
		else if (getAddressMode(1) == 2) rAccumulator = data & getRegister(rParameter1);
	}

	void CPU::ORR() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator |= data;
		else if (getAddressMode(1) == 1) rAccumulator = data | rParameter1;
		else if (getAddressMode(1) == 2) rAccumulator = data | getRegister(rParameter1);
	}

	void CPU::NOT() {
		if (getAddressMode(0) == 0) rAccumulator = ~rAccumulator;
		else if (getAddressMode(0) == 1) rAccumulator = ~rParameter1;
		else if (getAddressMode(0) == 2) rAccumulator = ~getRegister(rParameter1);
	}

	void CPU::XOR() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator ^= data;
		else if (getAddressMode(1) == 1) rAccumulator = data ^ rParameter1;
		else if (getAddressMode(1) == 2) rAccumulator = data ^ getRegister(rParameter1);
	}

	void CPU::NND() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator &= data;
		else if (getAddressMode(1) == 1) rAccumulator = data & rParameter1;
		else if (getAddressMode(1) == 2) rAccumulator = data & getRegister(rParameter1);
		rAccumulator = ~rAccumulator;
	}

	void CPU::NOR() {
		uint16_t data = 0;
		if (getAddressMode(0) == 1) data = rParameter1;
		else if (getAddressMode(0) == 2) data = getRegister(rParameter1);

		if (getAddressMode(1) == 0) rAccumulator |= data;
		else if (getAddressMode(1) == 1) rAccumulator = data | rParameter1;
		else if (getAddressMode(1) == 2) rAccumulator = data | getRegister(rParameter1);
		rAccumulator = ~rAccumulator;
	}

	void CPU::ROL() {
		uint16_t data = 0;
		uint16_t shift = 0;
		if (getAddressMode(0) == 1) shift = rParameter1;
		else if (getAddressMode(0) == 2) shift = getRegister(rParameter1);

		if (getAddressMode(1) == 0) data = rAccumulator;
		else if (getAddressMode(1) == 1) data = rParameter2;
		else if (getAddressMode(1) == 2) data = getRegister(rParameter2);
		
		rAccumulator = data << shift;
	}

	void CPU::ROR() {
		uint16_t data = 0;
		uint16_t shift = 0;
		if (getAddressMode(0) == 1) shift = rParameter1;
		else if (getAddressMode(0) == 2) shift = getRegister(rParameter1);

		if (getAddressMode(1) == 0) data = rAccumulator;
		else if (getAddressMode(1) == 1) data = rParameter2;
		else if (getAddressMode(1) == 2) data = getRegister(rParameter2);

		rAccumulator = data >> shift;
	}

	void CPU::jump() {
		if (getAddressMode(0) == 1) rProgramCounter = rParameter1;
		else if (getAddressMode(0) == 2) rProgramCounter += rParameter1;
		else if (getAddressMode(0) == 3) rProgramCounter -= rParameter1;
	}

	bool CPU::isZeroSet()
	{
		return (rStatusFlags & 0x0002) != 0;
	}

	bool CPU::isSignSet()
	{
		return (rStatusFlags & 0x0001) != 0;
	}

	bool CPU::isOverflowSet()
	{
		return (rStatusFlags & 0x0004) != 0;
	}

	bool CPU::isCarrySet()
	{
		return (rStatusFlags & 0x0008) != 0;
	}

	void CPU::JMP() {
		jump();
	}

	void CPU::JEQ() {
		if (isZeroSet()) jump();
	}

	void CPU::JNE() {
		if (!isZeroSet()) jump();
	}

	void CPU::JGT() {
		if (!isZeroSet() && isSignSet()) jump();
	}

	void CPU::JGE() {
		if (isZeroSet() || isSignSet()) jump();
	}

	void CPU::JLT() {
		if (!(isZeroSet() || isSignSet())) jump();
	}

	void CPU::JLE() {
		if (isZeroSet() || !isSignSet()) jump();
	}

	void CPU::REL() {
		if (getAddressMode(0) == 0) rRelativeAddress = 0;
		else if (getAddressMode(0) == 1) rRelativeAddress = rParameter1;
		else if (getAddressMode(0) == 2) rRelativeAddress = getRegister(rParameter1);
	}

	void CPU::CSR() {}

	void CPU::RTN() {
		stopExecution = true;
	}

	void CPU::SKP() {}

}