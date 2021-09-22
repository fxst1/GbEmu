#include "extra/GbPpu.hh"

gameboy::GbPpu::PixelFifo::PixelFifo(void):
	_queue()
{}
gameboy::GbPpu::PixelFifo::~PixelFifo(void){}

void	gameboy::GbPpu::PixelFifo::push(gameboy::Pixel const &p) {
	_queue.push(p);
}

gameboy::Pixel&	gameboy::GbPpu::PixelFifo::pop(void) {
	Pixel& p = _queue.front();
	_queue.pop();
	return (p);
}

void 	gameboy::GbPpu::PixelFifo::clear(void) {
	while (!_queue.empty()) _queue.pop();
}

bool	gameboy::GbPpu::PixelFifo::empty(void) const {
	return (_queue.empty());
}

bool	gameboy::GbPpu::PixelFifo::fill(void) const {
	return (_queue.size() == 16);
}

bool	gameboy::GbPpu::PixelFifo::enabled(void) const {
	std::size_t	size = _queue.size();
	return (size > 8 && size <= 16);
}

bool	gameboy::GbPpu::PixelFifo::canPush(void) const {
	std::size_t	size = _queue.size();
	return (size <= 8);
}
bool	gameboy::GbPpu::PixelFifo::canPop(void) const {
	std::size_t	size = _queue.size();
	return (size >= 8 && size <= 16);
}
