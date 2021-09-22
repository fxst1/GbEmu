#include "opcode/OpcodeBuilder.hh"

bool	opcode::Opcode::Comparator::operator()(Opcode const *a, Opcode const *b) const {
	return (a->bytecode() < b->bytecode());
}

opcode::Builder::DraftOpcode::DraftOpcode(std::string const &mne, bytecode_t bcode, bytecode_size_t bcode_len, bool fixed_len, int cycles, Placeholders const &holders, Executor const &executor):
	_fixed_len(fixed_len),
	_bytecode(bcode),
	_bytecode_size(bcode_len),
	_cycles(cycles),
	_mnemonic(mne),
	_holders(holders),
	_func(executor)
{
	/*
	std::cout << "Opcode: " << std::hex << _bytecode << ": " << this->_mnemonic
	<< " " << std::dec << (int)_bytecode_size
	<< " - " << std::dec << (int)_cycles
	<< std::endl;
	*/
}

opcode::Builder::DraftOpcode::~DraftOpcode(void) {
	opcode::Opcode*	o = nullptr;
	o->bytecode();
	std::cout << "DELETE" << std::endl;
}


void	opcode::Builder::DraftOpcode::operator()(void) const {
	if (this->_func != nullptr) {
		(this->_func)(this->_holders, *this);
	}
}

bool						opcode::Builder::DraftOpcode::isFixedBytecodeLength(void) const {
	return (this->_fixed_len);
}

opcode::bytecode_t			opcode::Builder::DraftOpcode::bytecode(void) const {
	return (this->_bytecode);
}
opcode::bytecode_size_t		opcode::Builder::DraftOpcode::bytecodeLength(void) const {
	return (this->_bytecode_size);
}
std::string					opcode::Builder::DraftOpcode::mnemonic(void) const {
	return (this->_mnemonic);
}
std::string					opcode::Builder::DraftOpcode::mnemonic(memunit::Memory &m, std::uintptr_t start) const {

	std::string		mne = this->mnemonic();
	std::size_t		pos = 0;

	while ((pos = mne.find("%")) != std::string::npos) {
		int					nbytes = 0;
		std::string			sub = mne.substr(pos + 1);
		const char*			buf = sub.c_str();

		std::size_t			index = 0;
		while (*(buf + index) && std::isdigit( *(buf + index) )) {
			nbytes *= 10;
			nbytes += *(buf + index) - '0';
			index++;
		}
		nbytes /= 8;
		std::stringstream		sstr;

		int v = 0;
		m.read(start + 1, (std::uint8_t*)&v, nbytes);
		sstr << "$";
		sstr << std::setw(nbytes * 2) << std::setfill('0') << std::hex << v;

		mne.replace(pos, sstr.str().length() + 1, sstr.str());
	}
	return (mne);
}
opcode::Placeholders		opcode::Builder::DraftOpcode::placeholders(void) const {
	return (this->_holders);
}
int							opcode::Builder::DraftOpcode::cycles(void) const {
	return (this->_cycles);
}

/****************************/

opcode::Builder::Builder(opcode::InstructionSet &instrset):
	_opcodes(instrset)
{

}

opcode::Builder::~Builder(void) {
}

/*****************************/

opcode::Builder&	opcode::Builder::begin(void) {
	this->_current.clear();
	this->_mnemonic = "";
	this->_bcode_val = 0;
	this->_bcode_callable = false;
	this->_exec_callable = false;
	this->_ncycles = 0;
	this->_holdermap.clear();
	return (*this);
}

opcode::Builder&	opcode::Builder::end(void) {
	opcode::Placeholders	tmpholders;
	this->build(tmpholders, 0);
	//std::cout << "============================" << std::endl;
	return (*this);
}

/*****************************/

opcode::Builder&	opcode::Builder::mnemonic(std::string const &mne) {
	this->_mnemonic = mne;
	return (*this);
}

/*****************************/

opcode::Builder&	opcode::Builder::placeholder(std::string const &macro, opcode::Placeholder const &holder) {
	auto it = this->_holdermap.find(macro);
	if (it == this->_holdermap.end()) {
		opcode::Placeholders	tmp;
		tmp.push_back(holder);
		this->_holdermap[macro] = tmp;
	} else {
		it->second.push_back(holder);
	}
	return (*this);
}
opcode::Builder&	opcode::Builder::placeholder(std::string const &macro, opcode::Placeholders const &holders) {
	this->_holdermap[macro] = holders;
	return (*this);
}
opcode::Builder&	opcode::Builder::placeholder(std::string const &macro, std::map<std::string, std::uint8_t*> mapper) {
	for (auto it = mapper.begin(); it != mapper.end(); it++) {
		opcode::Placeholder tmp;
		tmp.name = it->first;
		tmp.data = it->second;
		this->placeholder(macro, tmp);
	}
	return (*this);
}

/*****************************/

opcode::Builder&	opcode::Builder::bytecode(opcode::bytecode_t bcode, opcode::bytecode_size_t bcode_len, bool fixed_len) {
	this->_bcode_callable = false;
	this->_bcode_val = bcode;
	this->_bcode_len = bcode_len;
	this->_fixed_len = fixed_len;
	return (*this);
}
opcode::Builder&	opcode::Builder::bytecode(opcode::Builder::BytecodeGen const &bcodegen, opcode::bytecode_size_t bcode_len, bool fixed_len) {
	this->_bcode_callable = true;
	this->_bcode_func = bcodegen;
	this->_bcode_len = bcode_len;
	this->_fixed_len = fixed_len;
	return (*this);
}

/*****************************/

opcode::Builder&	opcode::Builder::call(opcode::Builder::Executor const &executor) {
	this->_exec_callable = true;
	this->_exec_func = executor;
	return (*this);
}
opcode::Builder&	opcode::Builder::cycles(unsigned int ncycles) {
	this->_ncycles = ncycles;
	return (*this);
}

opcode::InstructionSet	opcode::Builder::getOpcodes(void) const {
	return (this->_opcodes);
}

/*****************************/

opcode::Builder&	opcode::Builder::build(opcode::Placeholders const &holders, std::size_t pos) {

	if ((pos = this->_mnemonic.find("$", pos)) != std::string::npos) {

		std::string		mne_replacement = this->_mnemonic;
		std::size_t		end_pos = pos + 1;
		pos++;

		std::string		placeholder = "";
		while (end_pos < this->_mnemonic.size() && this->_mnemonic[end_pos] != ' ') end_pos++;

		placeholder = this->_mnemonic.substr(pos, end_pos - pos);


		auto it = this->_holdermap.find(placeholder);
		if (it != this->_holdermap.end()) {

			for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				opcode::Placeholders	tmp = holders;
				tmp.push_back(*it2);
				this->_mnemonic = mne_replacement.substr(0, pos - 1) +
								(*it2).name +
								mne_replacement.substr(end_pos);

				this->build(tmp, end_pos);
			}

		} else {
		}

		this->_mnemonic = mne_replacement;
	}

	if (pos == std::string::npos) {
		opcode::Builder::DraftOpcode* op = new	opcode::Builder::DraftOpcode(
			this->_mnemonic,
			this->_bcode_callable ? (this->_bcode_func)(holders) : this->_bcode_val,
			this->_bcode_len,
			this->_fixed_len,
			this->_ncycles,
			holders,
			this->_exec_func
		);
		this->_opcodes.insert(op);
	}
	return (*this);
}
