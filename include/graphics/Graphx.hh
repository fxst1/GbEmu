#ifndef GRAPHX_HH
# define GRAPHX_HH
# include <cstdint>
# include <cstdlib>
# include <string>

namespace	graphx {

	struct		Viewport {
		int		x;
		int		y;
		int		w;
		int		h;
	};

	class	Driver {
		public:
			virtual ~Driver(void) {}
			virtual void	setScreenTitle(std::string const &title) = 0;
			virtual void	resize(std::size_t w, std::size_t h) = 0;
			virtual void	setPixel(std::size_t x, std::size_t y, std::uint32_t rgb) = 0;
			virtual void	draw(void) = 0;
	};

	class	DriverAdapter: public graphx::Driver {
		private:
			std::string			_title;
			std::size_t			_w;
			std::size_t			_h;
			graphx::Viewport	_viewport;

		public:
			DriverAdapter(void);
			virtual ~DriverAdapter(void);

			void				setViewport(graphx::Viewport const &viewport);
			graphx::Viewport	getViewport(void) const;

			int				scaleX(int x) const;
			int				scaleY(int y) const;

			virtual void	setScreenTitle(std::string const &title);
			virtual void	resize(std::size_t w, std::size_t h);
			virtual void	setPixel(std::size_t x, std::size_t y, std::uint32_t rgb) = 0;
			virtual void	draw(void) = 0;
	};
};

#endif
