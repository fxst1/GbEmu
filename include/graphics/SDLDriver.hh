#ifndef GRAPHX_SDL_HH
# define GRAPHX_SDL_HH
# include "graphics/Graphx.hh"
# include <SDL2/SDL.h>
namespace	graphx {

	class	SDLDriver: public graphx::Driver {
		private:
			bool				_autoinit;
			SDL_Window*			_win;
			SDL_Renderer*		_renderer;

		public:
			SDLDriver(std::string const &title, std::size_t w, std::size_t h);
			SDLDriver(SDL_Window * win);
			~SDLDriver(void);


			void		setViewport(SDL_Rect const &vp);
			SDL_Rect	getViewport(void) const;


			void	setScreenTitle(std::string const &title);
			void	resize(std::size_t w, std::size_t h);
			void	setPixel(std::size_t x, std::size_t y, std::uint32_t rgb);
			void	draw(void);
	};

};

#endif
