#include "extra/GbMemUnit.hh"

gameboy::MemoryBank::MemoryBank(bool is_rom):
	_banks(),
	_bank(),
	_base(),
	_size(),
	_is_rom(is_rom)
{}

gameboy::MemoryBank::MemoryBank(std::uint8_t start, std::uint8_t end, std::uintptr_t base, std::size_t size):
	_banks(),
	_bank(),
	_base(base),
	_size(size)
{ this->init(start, end, base, size); }

gameboy::MemoryBank::MemoryBank(std::size_t nbanks, std::uintptr_t base, std::size_t size):
	_banks(),
	_bank(),
	_base(base),
	_size(size)
{ this->init(0, nbanks - 1, base, size); }

gameboy::MemoryBank::~MemoryBank(void) {
	for (auto it = this->_banks.begin(); it != this->_banks.end(); it++) {
		//std::cout << "Delete Bank: 0x" << std::hex << (int)it->first << ": " << *this << std::endl;
		delete it->second;
	}
}

/*****************************/

void			gameboy::MemoryBank::init(std::uint8_t start, std::uint8_t end, std::uintptr_t base, std::size_t size) {
	this->_base = base;
	this->_size = size;

	this->_bank = start;

	for (std::uint8_t i = start; i <= end; i++)
		this->_banks[ i ] = new std::uint8_t[ size ];
}

/*****************************/

std::size_t		gameboy::MemoryBank::allocSize(void) const {
	return (this->_size);
}
std::uintptr_t	gameboy::MemoryBank::base(void) const {
	return (this->_base);
}

/*****************************/

bool			gameboy::MemoryBank::canRead(void) const {
	return (true);
}
bool			gameboy::MemoryBank::canWrite(void) const {
	return (true);
}
bool			gameboy::MemoryBank::canExecute(void) const {
	return (true);
}

/*****************************/

std::uint8_t*	gameboy::MemoryBank::data(void) const {
	auto it = this->_banks.find(this->_bank);
	return (it == this->_banks.end() ? nullptr : it->second);
}

/*****************************/

std::uint8_t	gameboy::MemoryBank::getBank(void) const {
	std::cout << " GET BANK : " << std::dec << this->_bank << std::endl;
	return (this->_bank);
}
void			gameboy::MemoryBank::setBank(std::uint8_t bank) {
	std::cout << " UPDATE BANK : " << std::dec << this->_bank << " -> " << bank << std::endl;
	this->_bank = bank;
}
