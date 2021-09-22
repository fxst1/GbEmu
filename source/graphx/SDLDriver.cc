#include "graphics/SDLDriver.hh"
#include <iostream>
graphx::SDLDriver::SDLDriver(std::string const &title, std::size_t w, std::size_t h)
{
	std::cout << "SDL DRIVER: init: " << title << std::endl;
	this->_autoinit = true;
	SDL_CreateWindowAndRenderer(
		w, h, SDL_WINDOW_RESIZABLE,
		&_win, &_renderer
	);
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = w;
	r.h = h;
	this->setViewport(r);
	(void)title;
}

graphx::SDLDriver::SDLDriver(SDL_Window * win)
{
	std::cout << "SDL DRIVER: init" << std::endl;
	this->_autoinit = false;
	this->_win = win;
	this->_renderer = SDL_CreateRenderer(this->_win, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

graphx::SDLDriver::~SDLDriver(void) {
	SDL_DestroyRenderer(this->_renderer);
	if (this->_autoinit) {
		SDL_DestroyWindow(this->_win);
	}
}

void		graphx::SDLDriver::setViewport(SDL_Rect const &vp) {
	SDL_RenderSetViewport(this->_renderer, &vp);
}

SDL_Rect	graphx::SDLDriver::getViewport(void) const {
	SDL_Rect	vp;
	SDL_RenderGetViewport(this->_renderer, &vp);
	return (vp);
}

/*****************************/

void	graphx::SDLDriver::setScreenTitle(std::string const &title) {
	(void)title;
}
void	graphx::SDLDriver::resize(std::size_t w, std::size_t h) {
	(void)w;
	(void)h;
}
void	graphx::SDLDriver::setPixel(std::size_t x, std::size_t y, std::uint32_t argb) {
/*
	std::cout << "ARGB: " << std::hex << argb << std::endl;
	std::cout << "A:" << std::hex << ((argb >> 24) & 0xff) << std::endl;
	std::cout << "R:" << std::hex << ((argb >> 16) & 0xff) << std::endl;
	std::cout << "G:" << std::hex << ((argb >> 8) & 0xff) << std::endl;
	std::cout << "B:" << std::hex << (argb & 0xff) << std::endl;
	std::cout << "x: " << std::dec << x << ", " << std::dec << y << std::endl;
*/
	SDL_SetRenderDrawColor(this->_renderer, (argb >> 16) & 0xff, (argb >> 8) & 0xff, argb & 0xff, (argb >> 24) & 0xff);
	SDL_RenderDrawPoint(this->_renderer, x, y);
}
void	graphx::SDLDriver::draw(void) {
	/*SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 128);
	for (int i = 0; i < 256; i += 8) {
		SDL_RenderDrawLine(this->_renderer, 0, i, 256, i);
		SDL_RenderDrawLine(this->_renderer, i, 0, i, 256);
	}*/
	SDL_RenderPresent(this->_renderer);
}
