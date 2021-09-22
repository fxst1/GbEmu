#include "extra/GbMemUnit.hh"

gameboy::MBC::MBC(memunit::Memory & rom):
	memunit::MemoryMap(),
	_mode(RomBanking),
	_ram_enabled(false),
	_lowrom(*this, rom),
	_highrom(*this),
	_ram(0x03, 0xa000, 0x2000)
{
	this->set(&this->_lowrom);
	this->set(&this->_highrom);
	this->set(&this->_ram);
}

gameboy::MBC::~MBC(void) {
	//std::cout << "destroy MBC1" << std::endl;
}

/***********************************/

memunit::Memory*	gameboy::MBC::lorom(void) {
	return (&this->_lowrom);
}
memunit::Memory*	gameboy::MBC::hirom(void) {
	return (&this->_highrom);
}
memunit::Memory*	gameboy::MBC::externRam(void) {
	return (&this->_ram);
}

/*************************************/

gameboy::MBC::LowRom::LowRom(gameboy::MBC& mbc, memunit::Memory & rom):
	gameboy::MemoryBank(0x1, 0x0000, 0x4000),
	_mbc(mbc),
	_rom(rom)
{}
gameboy::MBC::LowRom::~LowRom(void){
	//std::cout << "destroy LowRom" << std::endl;
}

std::size_t		gameboy::MBC::LowRom::write(std::uintptr_t at, const std::uint8_t *src, std::size_t buflen) {

	for (std::size_t i = 0; i < buflen; i++) {

		// 0000-1FFF - RAM Enable (Write Only)
		if (0x0000 <= at && at <= 0x1fff) {
			this->_mbc._ram_enabled = ((*(src + i) & 0x0f) == 0x0A);
		}

		// 2000-3FFF - ROM Bank Number (Write Only)
		else if (0x2000 <= at && at <= 0x3fff) {
			this->setBank( ((this->getBank() & 0x3) << 5) | (*(src + i) & 0x1f) );
		}
		at++;
	}

	return (buflen);
	(void)src;
}

/***********************************/

gameboy::MBC::HiRom::HiRom(gameboy::MBC& mbc):
	gameboy::MemoryBank(0x1, 0x7f, 0x4000, 0x4000),
	_mbc(mbc)
{}
gameboy::MBC::HiRom::~HiRom(void){
	//std::cout << "destroy HiRom" << std::endl;
}

std::size_t		gameboy::MBC::HiRom::write(std::uintptr_t at, const std::uint8_t *src, std::size_t buflen) {

	for (std::size_t i = 0; i < buflen; i++) {

		// 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
		if (0x4000 <= at && at <= 0x5fff) {
			this->setBank(((*(src + i) & 0x3) << 5) | (this->getBank() & 0x1f));
		}

		// 6000-7FFF - ROM/RAM Mode Select (Write Only)
		else if (0x6000 <= at && at <= 0x7fff) {
			this->_mbc._mode = static_cast<gameboy::MBC::Mode>(*(src + i) & 0x1);
		}

		at++;
	}
	return (buflen);
	(void)src;
}
