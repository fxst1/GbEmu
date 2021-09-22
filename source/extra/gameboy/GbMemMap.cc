#include "extra/GbMemUnit.hh"

gameboy::GbMemMap::GbMemMap(void):
	memunit::MemoryMap(),
	_onboot(false),
	_bootrom(nullptr),

	_lorom(nullptr),
	_hirom(nullptr),
	_extern_ram(nullptr),

	_vram(nullptr),

	_wram(nullptr),
	_wram_switchable(nullptr),

	_echoram(nullptr),

	_oam(0x8000, 0x9fff),
	_not_usable(0x8000, 0x9fff),
	_io_registers(0x8000, 0x9fff),
	_hram(0xff80, 0xfffe),
	_interrupts(0xffff, 0xffff)
{}
gameboy::GbMemMap::~GbMemMap(void){}

memunit::Memory*			gameboy::GbMemMap::getVram(void) {
	return (_vram);
}

void			gameboy::GbMemMap::init(gameboy::GbGameLoader const &loader) {

	this->reset();

	memunit::Block* rom = new memunit::Block( loader.filebuf(), 0x0, loader.filesize() );

	gameboy::CartridgeType	type = loader.getCartridgeType();
	switch (type.mbc_version) {

		case gameboy::CartridgeType::Version::ROM:
		case gameboy::CartridgeType::Version::MBC1:
		case gameboy::CartridgeType::Version::MBC3:
			_mbc = new gameboy::MBC(*rom);
			break;
		default:
			_mbc = nullptr;
	}

	_lorom = _mbc->lorom();
	_hirom = _mbc->hirom();
	_extern_ram = _mbc->externRam();

	_bootrom = new memunit::Block((std::uint8_t*)gameboy::const_t::DmgBoot, 0x0, 0xff);
	//memunit::Block* logohack = new memunit::Block((std::uint8_t*)gameboy::const_t::NintendoLogo, 0x0104, sizeof(gameboy::const_t::NintendoLogo));

	_wram = new memunit::Block(0xC000, 0x1000);

	if (loader.needColorSupport()) {
		_vram = new gameboy::MemoryBank(0x0, 0x1, 0x8000, 0x2000);
		_wram_switchable = new gameboy::MemoryBank(0x1, 0x7, 0xd000, 0x1000);
	} else {
		_vram = new gameboy::MemoryBank(0x1, 0x8000, 0x2000);
		_wram_switchable = new gameboy::MemoryBank(0x1, 0x1, 0xd000, 0x1000);
	}

	_echoram = new memunit::Block(_wram->data(), 0xe000, 0x1dff);

	_onboot = true;

	//set(_mbc);
	//set(logohack);
	set(_vram);
	set(_extern_ram);
	set(_wram);
	set(_wram_switchable);
	set(_echoram);
	set(&_oam);
	set(&_not_usable);
	set(&_hram);
	set(&_interrupts);

	this->show();
}

void			gameboy::GbMemMap::reset(void) {
	/*
	if (_bootrom) delete _bootrom;
	if (_mbc) delete _mbc;
	if (_vram) delete _vram;
	if (_wram) delete _wram;
	if (_wram_switchable) delete _wram_switchable;
	if (_echoram) delete _echoram;
	*/
	this->clear();
}

std::size_t		gameboy::GbMemMap::write(std::uintptr_t at, const std::uint8_t *dst, std::size_t buflen) {
	if (_onboot && at == 0xff50)
	{
		for (std::size_t i = 0; i < buflen; i++)
			if (dst[i] != 0) {
				std::cout << "Disabling boot" << std::endl;
				_onboot = false; break;
			}
	}

	std::size_t	r = memunit::MemoryMap::write(at, dst, buflen);

	return (r);
}

std::size_t		gameboy::GbMemMap::read(std::uintptr_t at, std::uint8_t *dst, std::size_t buflen) {
	if (_onboot && _bootrom && at <= _bootrom->allocSize()) {

		std::size_t	len = this->_bootrom->read(at, dst, buflen);

		if (at >= 0x104 && at + buflen < 0x104 + sizeof(Const::NintendoLogo)) {
			::memcpy(dst, Const::NintendoLogo + (at - 0x104), buflen);
			len = buflen;
		}

		return (len);

	} else {

		std::size_t	len = 0;
		if (at >= 0x104 && at + buflen <= 0x104 + sizeof(Const::NintendoLogo)) {
			std::cout << "Read Logo at " << std::hex << at << " (" << std::dec << buflen << ")" << std::endl;
			std::cout << "Rel " << std::hex << (at - 0x104) << std::endl;
			::memcpy(dst, Const::NintendoLogo + (at - 0x104), buflen);
			len = buflen;
		} else {
			len = memunit::MemoryMap::read(at, dst, buflen);
		}
		return (len);
	}
}
