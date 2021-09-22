#ifndef GB_PPU_HH
# define GB_PPU_HH
# include "extra/GbMemUnit.hh"
# include "extra/GbData.hh"
# include "graphics/SDLDriver.hh"
# include "timing/Timing.hh"
# include <queue>
namespace	gameboy {

	class	GbPpu: public timing::Tick {

		public:

			class	LCDInterface {
				public:
					virtual ~LCDInterface(void) {}
					virtual void	drawPixel(int x, int y, gameboy::Pixel const & pix) = 0;
					virtual void 	render(void) = 0;
			};

			class	VideoRegisters: public memunit::Block {
				public:
					VideoRegisters(LCDRegisters &lcdregs);
					virtual ~VideoRegisters(void);

					virtual std::size_t		read(std::uintptr_t addr, std::uint8_t* buf, std::size_t buflen);
					virtual std::size_t		write(std::uintptr_t addr, const std::uint8_t* buf, std::size_t buflen);

					void	show(void) const;
					void	showLCDC(void) const;
					void	showSTAT(void) const;
					void	showSCXY(void) const;
					void	showLCLCY(void) const;

				private:
					LCDRegisters&	_lcdregs;
			};

			class	DMA: public memunit::Block {

				private:

					bool				_running;
					std::uintptr_t		_start;
					std::uint8_t		_offset;


				public:

					DMA(void);
					~DMA(void);

					void			start(std::uint8_t start);
					void			tick(void);

					memunit::MemoryMap*	_memmap;

			};

			class	PixelFifo {
				public:

					PixelFifo(void);
					~PixelFifo(void);

					bool				empty(void) const;
					bool				fill(void) const;
					bool				enabled(void) const;
					void				clear(void);
					void				push(gameboy::Pixel const &p);
					gameboy::Pixel&		pop(void);

					bool				canPush(void) const;
					bool				canPop(void) const;

				private:
					std::queue<gameboy::Pixel>	_queue;
			};

			struct	pixel_fetcher {
				int						_pix_x;
				int						_pix_y;

				std::uint16_t			_map_offset_x;
				std::uint16_t			_map_offset_y;

				std::uint16_t			_tile_line;
				std::uintptr_t			_tile_map;
				std::uintptr_t			_tile_data;
				std::uint16_t			_tile_index;
				std::uint8_t			_tile_num;
				std::uint8_t			_pix_data[2];

				int						_cycle;
			};

			GbPpu(void);
			~GbPpu(void);

			void	setMemory(memunit::MemoryMap& map);
			void	setVram(memunit::Memory* vram);
			void	setDriver(gameboy::GbPpu::LCDInterface * disp);
			gameboy::GbPpu::LCDInterface* 	getDriver(void);
			void	tick(void);


		private:

			LCDRegisters				_regs;
			VideoRegisters				_regblk;

			DMA							_dma;
			LCDInterface*				_display;
			memunit::MemoryMap*			_memmap;
			memunit::Memory*				_vram;

			struct Oam					_sprites[10];
			struct Oam*					_sprites_cur;

			struct pixel_fetcher		_pixfetcher;
			PixelFifo					_pixfifo;

			int							_shift_x;
			bool						_on_win;

			int							_x;
			int							_y;
			int							_cycle;


			void	pixelTransferTick(void);
			void	fetchTile(void);
			void	fetchTileDataHigh(void);
			void	fetchTileDataLow(void);
			void	fetchPush(void);
			void	sendLCD(void);

			void 	dumpVRAM(void) const;
			void 	dumpTiles(std::uintptr_t tile_map, std::uintptr_t tile_data) const;

			int		nTickPerFrame(void) const;
	};
};

#endif
