#include "extra/GbCpu.hh"

gameboy::GbCpu::GbCpu(gameboy::States &stat):
	_stat(stat),
	_memmap(nullptr),
	_opcodes(),
	_cycles_count(0)
{
	bzero(&_regs, sizeof(_regs));
	stat._cpuregs = &_regs;
	this->prepare();
}

gameboy::GbCpu::~GbCpu(void) {
}

/*********************************/

gameboy::CpuRegisters*		gameboy::GbCpu::regs(void) {
	return (&_regs);
}

int 	gameboy::GbCpu::nTickPerFrame(void) const {
	return (1);
}

void 	gameboy::GbCpu::fetchNext(void) {

	std::uint32_t		code = 0;
	opcode::bytecode_t	opcode = 0;
	opcode::Opcode*		instruction = nullptr;


	_memmap->read(_regs.pc, (std::uint8_t*)&code, sizeof(code));

	opcode = code & 0xff;
	if (opcode == 0xcb) {
		opcode = ((code & 0xff00) >> 8) | 0xcb00;
	}
	instruction = this->_opcodes.find(opcode);
	if (instruction != nullptr) {
		_regs.pc += instruction->bytecodeLength();
	}
}

void 	gameboy::GbCpu::fetchNext(opcode::Opcode const & instruction) {
	_regs.pc += instruction.bytecodeLength();
}

void	gameboy::GbCpu::tick(void) {

	if (_cycles_count <= 0) {

		std::uint16_t		code = 0;
		opcode::bytecode_t	opcode = 0;
		opcode::Opcode*		instruction = nullptr;


		_memmap->read(_regs.pc, (std::uint8_t*)&code, sizeof(code));

		opcode = code & 0xff;
		if (opcode == 0xcb) {
			opcode = ((code & 0xff00) >> 8) | 0xcb00;
		}

		instruction = this->_opcodes.find(opcode);
		if (instruction != nullptr) {

			std::uint16_t	pc = _regs.pc;

			(*instruction)();

			if (pc == _regs.pc)
				_regs.pc += instruction->bytecodeLength();

			if (pc >= 0xf1) {
				std::cout << "------------------ " << std::hex << pc << std::endl;
				std::cout << std::hex << opcode << " - " << std::hex << code << ": " << std::endl;;
				std::cout << instruction->mnemonic(*_memmap, pc) << std::endl;
				this->showRegs();
				std::cout << std::endl;
			}

			_cycles_count = (instruction->cycles()) - 1;

		} else {
			_stat.log("error") << "GbCpu: Instruction not exists: 0x" << std::hex << (int)opcode << std::endl;
			_stat.log("error") << " PC = @0x" << std::hex << (int)_regs.pc << std::endl;
			_stat.stop();
		}

	} else {
		//std::cout << "GbCpu: Wait for " << std::dec << _cycles_count << std::endl;
		_cycles_count--;
	}
}

void	gameboy::GbCpu::setMemory(memunit::Memory &memory) {
	this->_memmap = &memory;
}

opcode::InstructionSet		gameboy::GbCpu::opcodes(void) const {
	return (this->_opcodes);
}

void 						gameboy::GbCpu::showRegs(void) const {
	std::uint8_t			val = 0;
	std::uint8_t*			buf = (std::uint8_t*)&val;

//	_memmap->read(_regs.bc, buf, sizeof(val));
	std::cout << "B:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.b << " | ";
	std::cout << "C:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.c;
//	std::cout << " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val;
	std::cout << std::endl;

//	_memmap->read(_regs.de, buf, sizeof(val));
	std::cout << "D:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.d << " | ";
	std::cout << "E:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.e;
//	std::cout << " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val;
	std::cout << std::endl;

//	_memmap->read(_regs.hl, buf, sizeof(val));
	std::cout << "H:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.h << " | ";
	std::cout << "L:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.l;
//	std::cout << " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val;
	std::cout << std::endl;

//	_memmap->read(_regs.sp, buf, sizeof(val));
	std::cout << "SP: $" << std::hex << std::setfill('0') << std::setw(4) << (int)_regs.sp;
//	std::cout << " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val;
	std::cout << std::endl;

//	_memmap->read(_regs.pc, buf, sizeof(val));
	std::cout << "PC: $" << std::hex << std::setfill('0') << std::setw(4) << (int)_regs.pc;
//	std::cout << " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val;
	std::cout << std::endl;

	std::cout << "A:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.a << " | ";
	std::cout << "F:  $" << std::hex << std::setfill('0') << std::setw(2) << (int)_regs.f;
	std::cout << std::hex << (memunit::bitval<CpuRegisters::carry>(_regs.f) ? "  C" : " !C")
			<< std::hex << (memunit::bitval<CpuRegisters::half_carry>(_regs.f) ? "  H" : " !H")
			<< std::hex << (memunit::bitval<CpuRegisters::add>(_regs.f) ? "  N" : " !N")
			<< std::hex << (memunit::bitval<CpuRegisters::zero>(_regs.f) ? "  Z" : " !Z") << std::endl;
	(void)val;
	(void)buf;
}

void 						gameboy::GbCpu::showStack(void) const {

	std::uint32_t			ptr = _regs.sp;
	std::uint8_t			val = 0;
	std::uint8_t*			buf = (std::uint8_t*)&val;

	while (ptr <= 0xfffe) {
		_memmap->read(ptr, buf, sizeof(std::uint8_t));
		std::cout << "$" << std::hex << std::setfill('0') << std::setw(4) << (int)ptr
			<< " -> " << std::hex << std::setfill('0') << std::setw(2) << (int)val
			<< std::endl;
		ptr += 2;
	}

	(void)val;
	(void)buf;
}
