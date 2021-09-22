#include "graphics/NCursesDriver.hh"
#include <iostream>
graphx::NCursesDriver::NCursesDriver(std::size_t w, std::size_t h, std::size_t x, std::size_t y)
{
	_win = ::subwin(stdscr, h, w, y, x);
	_w = w;
	_h = h;
	::box(_win, ACS_VLINE, ACS_HLINE);
}

graphx::NCursesDriver::~NCursesDriver(void) {
	free(_win);
}

void	graphx::NCursesDriver::draw(void) {
	::wrefresh(_win);
}

/*****************************/

void	graphx::NCursesDriver::setScreenTitle(std::string const &title) {
	(void)title;
}
void	graphx::NCursesDriver::resize(std::size_t w, std::size_t h) {
	(void)w;
	(void)h;
}
void	graphx::NCursesDriver::setPixel(std::size_t x, std::size_t y, std::uint32_t color_pair_index) {
	if (x < _w && y < _h) {
		::attron(color_pair_index);

		::mvwprintw(_win, y, x, " ");

		::attroff(color_pair_index);
	}
}
