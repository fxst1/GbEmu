
#include "extra/GbPpu.hh"

gameboy::GbPpu::VideoRegisters::VideoRegisters(LCDRegisters &lcdregs):
	memunit::Block((std::uint8_t*)&lcdregs, 0xff40, sizeof(_regs)),
	_lcdregs(lcdregs)
{(void)lcdregs;}
gameboy::GbPpu::VideoRegisters::~VideoRegisters(void){}

std::size_t		gameboy::GbPpu::VideoRegisters::read(std::uintptr_t addr, std::uint8_t* buf, std::size_t buflen) {
	return (memunit::Block::read(addr, buf, buflen));
	(void)_lcdregs;
}

std::size_t		gameboy::GbPpu::VideoRegisters::write(std::uintptr_t addr, const std::uint8_t* buf, std::size_t buflen) {

	std::size_t ret = memunit::Block::write(addr, buf, buflen);

	std::cout << "* Update video requested" << std::endl;
	memunit::MemoryMap::bufferPrintMemory(std::cout, buf, buflen, addr);
	if (addr == 0xff40) this->showLCDC();
	else if (addr == 0xff41) this->showSTAT();
	else if (addr == 0xff42 || addr == 0xff43) this->showSCXY();
	else if (addr == 0xff44 || addr == 0xff45) this->showLCLCY();

	return (ret);
	(void)_lcdregs;
}
void			gameboy::GbPpu::VideoRegisters::showLCDC(void) const {
	std::cout << "0xff40: LCD Controller:" << std::endl;
	std::cout << " - LDC enabled : " << (memunit::bitval<LCDRegisters::lcd_enabled>(_lcdregs.lcdc) ? "ON" : "OFF") << std::endl;
	std::cout << " - Window tile map area : " << (memunit::bitval<LCDRegisters::win_tilemap>(_lcdregs.lcdc) ? "9800-9BFF" : "9C00-9FFF") << std::endl;
	std::cout << " - Window enabled : " << (memunit::bitval<LCDRegisters::win_tilemap_enabled>(_lcdregs.lcdc) ? "ON" : "OFF") << std::endl;
	std::cout << " - BG and Window tile data area : " << (memunit::bitval<LCDRegisters::bgwin_tiledata>(_lcdregs.lcdc) ? "8800-97FF" : "8000-8FFF") << std::endl;
	std::cout << " - BG tile map area : " << (memunit::bitval<LCDRegisters::bg_tilemap>(_lcdregs.lcdc) ? "9800-9BFF" : "9C00-9FFF") << std::endl;
	std::cout << " - OBJ size : " << (memunit::bitval<LCDRegisters::sprite_size>(_lcdregs.lcdc) ? "8x8" : "16x16") << std::endl;
	std::cout << " - OBJ enable : " << (memunit::bitval<LCDRegisters::sprite_enabled>(_lcdregs.lcdc) ? "ON" : "OFF") << std::endl;
	std::cout << " - BG and Window enable/priority : " << (memunit::bitval<LCDRegisters::bgwin_priority>(_lcdregs.lcdc) ? "ON" : "OFF") << std::endl;
}

void			gameboy::GbPpu::VideoRegisters::showSTAT(void) const {
	std::cout << "0xff41: LCD Status:" << std::endl;
	std::cout << " - <Unused>" << std::endl;
	std::cout << " - LYC=LY STAT Interrupt : " << (memunit::bitval<LCDRegisters::ly_lyc_int>(_lcdregs.stat) ? "ON" : "OFF") << std::endl;
	std::cout << " - Mode 2 OAM STAT Interrupt : " << (memunit::bitval<LCDRegisters::oam_int>(_lcdregs.stat) ? "ON" : "OFF") << std::endl;
	std::cout << " - Mode 1 VBlank STAT Interrupt : " << (memunit::bitval<LCDRegisters::vblank_int>(_lcdregs.stat) ? "ON" : "OFF") << std::endl;
	std::cout << " - Mode 0 HBlank STAT Interrupt : " << (memunit::bitval<LCDRegisters::hblank_int>(_lcdregs.stat) ? "ON" : "OFF") << std::endl;
	std::cout << " - LYC=LY Flag : " << (memunit::bitval<LCDRegisters::ly_lyc>(_lcdregs.stat) ? "==" : "!=") << std::endl;

	std::string mode = "";
	switch (memunit::bitval<LCDRegisters::mode, 2>(_lcdregs.stat)) {
		case 0x0: mode = "HBlank"; break;
		case 0x1: mode = "VBlank"; break;
		case 0x2: mode = "Search OAM"; break;
		default: mode = "Transferring Data"; break;
	}
	std::cout << " - Mode Flag : " << mode << std::endl;
}

void			gameboy::GbPpu::VideoRegisters::showSCXY(void) const {
	std::cout << "0xff42: SC Y : " << std::endl;
	std::cout << "0xff43: SC X : " << std::endl;
	memunit::MemoryMap::bufferPrintMemory(std::cout, reinterpret_cast<std::uint8_t*>(&_lcdregs.scy), 2, 0xff42);
}

void			gameboy::GbPpu::VideoRegisters::showLCLCY(void) const {
	std::cout << "0xff44: LY   : " << std::hex << std::setw(2) << std::setfill('0') << _lcdregs.ly << std::endl;
	std::cout << "0xff45: LYC  : " << std::hex << std::setw(2) << std::setfill('0') << _lcdregs.lyc << std::endl;
	memunit::MemoryMap::bufferPrintMemory(std::cout, reinterpret_cast<std::uint8_t*>(&_lcdregs.ly), 2, 0xff44);
}

void			gameboy::GbPpu::VideoRegisters::show(void) const {
	this->showLCDC();
	std::cout << std::endl;

	this->showSTAT();
	std::cout << std::endl;

	this->showSCXY();
	std::cout << std::endl;

	this->showLCLCY();
	std::cout << std::endl;
}
