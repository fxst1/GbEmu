#include "extra/GbPpu.hh"

gameboy::GbPpu::DMA::DMA(void):
 	_running(false), _start(0), _offset(0), _memmap(nullptr) {}
gameboy::GbPpu::DMA::~DMA(void){}

void	gameboy::GbPpu::DMA::start(std::uint8_t start) {
	this->_start = start << 8;
	this->_offset = 0;
	this->_running = true;
}

void	gameboy::GbPpu::DMA::tick(void) {
	if (this->_running) {

		std::uint8_t	byte = 0;
		_memmap->read(this->_start | this->_offset, &byte, sizeof(byte));
		_memmap->write(0xfe | this->_offset, &byte, sizeof(byte));

		if (this->_offset >= 0x9F) {
			this->_running = false;
		} else
			this->_offset++;
	}
}
