#pragma once
# include "graphics/Graphx.hh"
# include <ncurses.h>
namespace	graphx {

	class	NCursesDriver: public graphx::Driver {
		private:
			::WINDOW*			_win;
			std::size_t			_w;
			std::size_t			_h;
			std::uint32_t*		_buffer;

		public:
			NCursesDriver(std::size_t w, std::size_t h, std::size_t x, std::size_t y);
			~NCursesDriver(void);

			void	draw(void);
			void	setPixel(std::size_t x, std::size_t y, std::uint32_t rgb);

			void	setScreenTitle(std::string const &title);
			void	resize(std::size_t w, std::size_t h);
	};

};
