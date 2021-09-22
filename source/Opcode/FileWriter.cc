#include "opcode/Assembler.hh"

opcode::FileWriter::FileWriter(opcode::InstructionSet const &set):
	_opcodes(set){

}
opcode::FileWriter::~FileWriter(void){
}

void				opcode::FileWriter::decode(std::ostream &output, std::uint8_t *src, std::size_t len) {

	std::size_t	i = 0;

	while ( i < len ) {

		opcode::Opcode*		instr = nullptr;
		opcode::bytecode_t	bcode = 0;
		opcode::bytecode_t	orig_bcode = -1;

		::memcpy((char*)&orig_bcode, src + i, sizeof(orig_bcode));

		std::cout << "== " << std::dec << (int)i << std::endl;
		std::cout << std::hex << (int)orig_bcode << std::endl;

		bcode = orig_bcode;

		std::size_t	bit = 0;
		while (bit < 32 &&  instr == nullptr) {
			bcode = orig_bcode >> bit;
			std::cout << "Test: " << std::hex << (int)bcode << std::endl;
			instr = _opcodes.find(bcode);
			bit += 8;
		}

		if (instr == nullptr) {
			output << "# Illegal code : " << std::hex << (int)orig_bcode << std::endl;
			break;
		}

		std::uint8_t*	tmp = new std::uint8_t[ instr->bytecodeLength() ];
		::memcpy((char*)tmp, src + i, instr->bytecodeLength());

		std::cout << "OK: " << std::hex << (int)bcode << std::endl;
//		std::cout << "OP: " << instr->mnemonic( tmp ) << std::endl;


//		output << instr->mnemonic( tmp ) << "\t # " << std::hex << (int)bcode << std::endl;
		delete[] tmp;

		i += instr->bytecodeLength();
	}
}

void				opcode::FileWriter::decode(std::ostream &output, std::istream &input) {

	if (input.fail()) {
		std::cerr << "NOT OPEN" << std::endl;
		return ;
	}

	std::size_t	i = 0;

	while ( !input.eof() ) {

		opcode::Opcode*		instr = nullptr;
		opcode::bytecode_t	bcode = 0;
		opcode::bytecode_t	orig_bcode = -1;

		input.seekg(i, std::ios::beg);
		input.read((char*)&orig_bcode, sizeof(orig_bcode));

		std::cout << "== " << std::dec << (int)i << std::endl;
		std::cout << std::hex << (int)orig_bcode << std::endl;

		bcode = orig_bcode;

		std::size_t	bit = 0;
		while (bit < 32 &&  instr == nullptr) {
			bcode = orig_bcode >> bit;
			std::cout << "Test: " << std::hex << (int)bcode << std::endl;
			instr = _opcodes.find(bcode);
			bit += 8;
		}

		if (instr == nullptr) {
			output << "# Illegal code : " << std::hex << (int)orig_bcode << std::endl;
			break;
		}

		std::uint8_t*	tmp = new std::uint8_t[ instr->bytecodeLength() ];
		input.seekg(i, std::ios::beg);
		input.read((char*)tmp, instr->bytecodeLength());

		std::cout << "OK: " << std::hex << (int)bcode << std::endl;
	//	std::cout << "OP: " << instr->mnemonic( tmp ) << std::endl;


	//	output << instr->mnemonic( tmp ) << "\t # " << std::hex << (int)bcode << std::endl;
		delete[] tmp;

		i += instr->bytecodeLength();
	}
}
