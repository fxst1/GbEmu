#include "opcode/OpcodeBuilder.hh"

opcode::InstructionSet::OpcodeHint::OpcodeHint(opcode::bytecode_t bcode):
	_bytecode(bcode){}
opcode::InstructionSet::OpcodeHint::~OpcodeHint(void){}

opcode::bytecode_t 		opcode::InstructionSet::OpcodeHint::bytecode(void) const {
	return (_bytecode);
}
opcode::bytecode_size_t opcode::InstructionSet::OpcodeHint::bytecodeLength(void) const {
	return (1);
}
std::string 			opcode::InstructionSet::OpcodeHint::mnemonic(void) const {
	return ("<OpcodeHint>");
}
std::string				opcode::InstructionSet::OpcodeHint::mnemonic(memunit::Memory &m, std::uintptr_t start) const {
	return ("<OpcodeHint>"); (void)start; (void)m;
}
int						opcode::InstructionSet::OpcodeHint::cycles(void) const {
	return (0);
}
void					opcode::InstructionSet::OpcodeHint::operator()(void) const {}

/**************************************/

opcode::InstructionSet::InstructionSet(void):
	_opcodes(){}
opcode::InstructionSet::InstructionSet(opcode::InstrSet const &set):
	_opcodes(set){}
opcode::InstructionSet::~InstructionSet(void){
	for (opcode::Opcode * op : _opcodes) {
		//delete op;
	}
}

opcode::Opcode*		opcode::InstructionSet::find(opcode::bytecode_t bcode) const {
	opcode::InstructionSet::OpcodeHint	hint(bcode);
	auto it = this->_opcodes.find(&hint);
	if (it == this->_opcodes.end()) return (nullptr);
	return (*it);
}

opcode::InstrSet::iterator	opcode::InstructionSet::begin(void) {
	return this->_opcodes.begin();
}

opcode::InstrSet::iterator	opcode::InstructionSet::end(void) {
	return this->_opcodes.end();
}

void	opcode::InstructionSet::insert(opcode::Opcode *op) {
	if (op != nullptr) {
		this->_opcodes.insert(op);
	} else {
		std::cerr << "NULL INSTER"  << std::endl;
	}
}

void	opcode::InstructionSet::dump(void) const {
	for (Opcode *op: _opcodes) {
		if (op != nullptr) {
			std::cout << " - "
			 << std::hex << op->bytecode()
					<< " (" << std::dec << op->bytecodeLength() << ") : " << op->mnemonic() << std::endl;
		} else {
			std::cout << "NULL" << std::endl;
		}
	}
}

void 	opcode::InstructionSet::dissas(std::uint16_t pc, memunit::MemoryMap& memmap, std::ostream & out) const {
	std::uint32_t		code = 0;
	opcode::bytecode_t	opcode = 0;
	opcode::Opcode*		instruction = nullptr;
	std::size_t			max_offset = memmap.allocSize();

	memmap.printMemory(out);

	pc = 0x31;

	do {
		memmap.read(pc, (std::uint8_t*)&code, sizeof(code));

		opcode = code & 0xff;
		if (opcode == 0xcb) {
			opcode = ((code & 0xff00) >> 8) | 0xcb00;
		}

		instruction = this->find(opcode);
		if (instruction != nullptr) {

/*			std::cout << std::hex << _regs.pc << ": 0x"
				<< std::hex << (int)opcode
				<< ": " << instruction->mnemonic(*_memmap, _regs.pc) << std::endl;
*/
			out << instruction->mnemonic(memmap, pc)
				<< " # " << std::hex << pc
				<< std::endl;

			pc += instruction->bytecodeLength();

		} else {
			out << "# " << std::hex << pc
				<< " -> " << std::hex << opcode
				<< std::endl;

			pc++;
		}

	} while (pc < max_offset);
}
