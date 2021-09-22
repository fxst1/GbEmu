#include "extra/GbPpu.hh"

gameboy::GbPpu::GbPpu(void):
	_regs(),
	_regblk(_regs),

	_dma(),
	_display(nullptr),
	_memmap(nullptr),
	_vram(nullptr),

	_sprites(),
	_sprites_cur(_sprites),

	_pixfetcher(),
	_pixfifo(),

	_shift_x(0),
	_on_win(false),

	_x(0),
	_y(0),
	_cycle(0)
{
	::bzero(&_pixfetcher, sizeof(_pixfetcher));
}


gameboy::GbPpu::~GbPpu(void){}

gameboy::GbPpu::LCDInterface* 	gameboy::GbPpu::getDriver(void) {
	return (_display);
}
void	gameboy::GbPpu::setDriver(gameboy::GbPpu::LCDInterface * drv) {
	this->_display = drv;
}
int		gameboy::GbPpu::nTickPerFrame(void) const {
	return (4);
}
void	gameboy::GbPpu::tick(void) {

	if (!memunit::bitval<LCDRegisters::lcd_enabled>(_regs.lcdc)) {
		_regs.stat = (_regs.stat & 0xfc) | VBLANK;
		_cycle = 0;
		return;
	}

	//this->_dma.tick();

	std::uint8_t mode = _regs.stat & 0x03;

	/*
	** 4 bytes per sprite
	** 20 cycles
	** from 0xfe00 to 0xfe9f (160 bytes)
	** ---> sprite per cycles = 160 / (20 * 4) = 2
	*/

	if (mode == OAM_SEARCH) {

		std::size_t		size = memunit::bitval<LCDRegisters::sprite_size>(_regs.lcdc) ? 8 : 16;
		std::uintptr_t	addr_obj_attr = 0xfe00 + (_cycle * 4);
		struct Oam		sprite;

		if (_sprites_cur < _sprites + 10) {

			_memmap->read(addr_obj_attr, reinterpret_cast<std::uint8_t*>(&sprite), sizeof(sprite));
			if (sprite.x > 0 && _regs.ly + 16 >= sprite.y && _regs.ly + 16 < sprite.y + size) {
				*_sprites_cur = sprite;
				_sprites_cur++;
			}

		}

		if (_sprites_cur < _sprites + 10) {

			_memmap->read(addr_obj_attr, reinterpret_cast<std::uint8_t*>(&sprite), sizeof(sprite));
			if (sprite.x > 0 && _regs.ly + 16 >= sprite.y && _regs.ly + 16 < sprite.y + size) {
				*_sprites_cur = sprite;
				_sprites_cur++;
			}

		}

		_cycle++;

		/* Init pixel transfert */
		if (_cycle >= 20) {
			_regs.stat = (_regs.stat & 0xfc) | PIXEL_TRANSFER;
			_on_win = false;

			_x = 0;
			_pixfetcher._tile_index = 0;

//			_pixfetcher._tile_map += ((_pixfetcher._map_offset_x / 8) % 32) + ((((_pixfetcher._map_offset_y / 8)) * 32));
//			_pixfetcher._tile_map += _pixfetcher._tile_index + 32 * (_pixfetcher._tile_line / 8);

			_pixfetcher._cycle = 0;
		}

	} else if (mode == PIXEL_TRANSFER) {

		_cycle++;
		if (_pixfifo.canPush()) {
			this->pixelTransferTick();
		}

		if ( _pixfifo.canPop() ) {
			this->sendLCD();
			_x++;
		}

		if (_x >= 160) {
			_cycle = 0;
			_regs.stat = (_regs.stat & 0xfc) | HBLANK;
//			std::cout << "Cycle " << std::dec << _cycle << " -> HBLANK" << std::endl;
//			std::cout << "x = " << std::dec << _x << std::endl;
		}

	} else if (mode == HBLANK) {

		if (!_pixfifo.empty()) this->sendLCD();

		_cycle++;
		if (_cycle >= 51) {
			if (_y >= 143) {
//				std::cout << "Cycle " << std::dec << _cycle << " -> VBLANK" << std::endl;
//				std::cout << "y = " << std::dec << _y << std::endl;

				_regs.stat = (_regs.stat & 0xfc) | VBLANK;
				_pixfetcher._tile_line = 0;
			} else {
//				std::cout << "Cycle " << std::dec << _cycle << " -> OAM_SEARCH" << std::endl;
				_regs.stat = (_regs.stat & 0xfc) | OAM_SEARCH;
				_pixfetcher._tile_line++;
			}
			_cycle = 0;
			_x = 0;
			_y++;
		}

	} else {

		/*
		**	VBlank
		*/

		_cycle++;

		if (_cycle >= 1140) {
//			std::cout << "Cycle " << std::dec << _cycle << " -> OAM_SEARCH" << std::endl;
			_cycle = 0;
			_y = 0;
			_x = 0;
			_pixfetcher._map_offset_x = _regs.scx;
			_pixfetcher._map_offset_y = _regs.scy;

			_regs.stat = (_regs.stat & 0xfc) | OAM_SEARCH;

			if (_display) {
				_display->render();
			}

		}
	}

	/*
	**	Update LY
	*/
	_regs.ly = _y;
	if (_regs.lyc == _y) {
		_regs.stat |= 0x4;
	} else {
		_regs.stat &= ~0x4;
	}
}

void	gameboy::GbPpu::setVram(memunit::Memory* vram) {
	_vram = vram;
}

void	gameboy::GbPpu::setMemory(memunit::MemoryMap & map) {
	map.set(&_dma);
	map.set(&_regblk);
	_memmap = &map;
	_dma._memmap = &map;
}

/*************************/


void	gameboy::GbPpu::pixelTransferTick(void) {

	/*
	** When Window X == current x on current scanline
	*/

	switch (_pixfetcher._cycle) {
		case 0:
			this->fetchTile();
			break;

		case 2:
			this->fetchTileDataLow();
			break;

		case 4:
			this->fetchTileDataHigh();
			break;

		case 6:
			this->fetchPush();
			break;

		default:
			break;
	}

	_pixfetcher._cycle++;
	if (_pixfetcher._cycle >= 7) _pixfetcher._cycle = 0;
}

void 	gameboy::GbPpu::fetchTile(void) {

	_pixfetcher._tile_map = 0x9800;
	_pixfetcher._tile_map += (((_pixfetcher._map_offset_x / 8) + _pixfetcher._tile_index) % 32)
		+ ( ((_pixfetcher._map_offset_y + _pixfetcher._tile_line ) / 8) * 32);

//	_pixfetcher._tile_map += ((_pixfetcher._map_offset_x / 8) % 32) + ((((_pixfetcher._map_offset_y / 8)) * 32));
//	_pixfetcher._tile_map += _pixfetcher._tile_index + 32 * (_pixfetcher._tile_line / 8);


	_vram->read(_pixfetcher._tile_map, &_pixfetcher._tile_num, sizeof(_pixfetcher._tile_num));
//	std::cout << "GbPpu PixFetcher: Tile:" << std::hex << (int)(_pixfetcher._tile_map + _pixfetcher._tile_index) << " -> " << std::hex << (int)(_pixfetcher._tile_num)  << std::endl;
//	std::cout << "\tTile Index:" << std::hex << (int)_pixfetcher._tile_index << std::endl;
}

void 	gameboy::GbPpu::fetchTileDataHigh(void) {

	_vram->read(_pixfetcher._tile_data + 1, &_pixfetcher._pix_data[1], sizeof(_pixfetcher._pix_data[1]));

//	std::cout << "GbPpu PixFetcher: Data H: " << std::hex << (int)_pixfetcher._tile_data + 1 <<  " -> " << std::hex << (int)(_pixfetcher._pix_data[1]) << std::endl;
}

void 	gameboy::GbPpu::fetchTileDataLow(void) {
	std::uintptr_t	offset = 0x8000;
	offset += _pixfetcher._tile_num * 16 + (((_pixfetcher._map_offset_y + _pixfetcher._tile_line) % 8) * 2);
	_pixfetcher._tile_data = offset;

	_vram->read(_pixfetcher._tile_data, &_pixfetcher._pix_data[0], sizeof(_pixfetcher._pix_data[0]));

//	std::cout << "GbPpu PixFetcher: Data L: " << std::hex << (int)_pixfetcher._tile_data << " -> " << std::hex << (int)(_pixfetcher._pix_data[0]) << std::endl;
}

void 	gameboy::GbPpu::fetchPush(void) {

	//std::cout << "GbPpu PixFetcher: Push" << std::endl;

	for (int bit = 7; bit >= 0; bit--) {

		std::uint8_t color_num = (((_pixfetcher._pix_data[0] >> bit) & 0x1)) | (((_pixfetcher._pix_data[1] >> bit) << 1) & 0x1);

		Pixel p;

		p.color = (_regs.bgp >> (color_num * 2)) & 0x3;
		p.pal = color_num;
		p.x = (_pixfetcher._tile_index * 8) + (7 - bit);
		p.y = _y;
/*
		//if (p.color == 0) {
			std::cout << "Pixel" << std::endl;
			std::cout << "map    : " << std::hex << (int)_pixfetcher._tile_map << std::endl;
			std::cout << "data   : " << std::hex << (int)_pixfetcher._tile_data << std::endl;
			std::cout << " " << std::hex << (((_pixfetcher._pix_data[0] >> bit) & 0x1)) << std::endl;
			std::cout << " " << std::hex << (((_pixfetcher._pix_data[1] >> bit) & 0x1)) << std::endl;
			std::cout << "color #" << std::hex << p.color << std::endl;
		//}
	*/
		//std::cout << "PUSH : x: " << std::dec << p.x << ", y: " << std::dec << p.y << std::endl;
		_pixfifo.push(p);
	}
	_pixfetcher._tile_index++;
}

void	gameboy::GbPpu::sendLCD() {

	struct Pixel& p =  _pixfifo.pop();

	if (_display) {
		_display->drawPixel(p.x, p.y, p);
		//std::cout << "POP : x: " << std::dec << p.x << ", y: " << std::dec << p.y << std::endl;
	}
}

void 	gameboy::GbPpu::dumpVRAM(void) const {
	_memmap->printMemory(std::cout, 0x8000, 0x2000);
//	for (std::uintptr_t addr = 0x8000; addr <= 0x)
}

void 	gameboy::GbPpu::dumpTiles(std::uintptr_t tile_map, std::uintptr_t tile_data) const {
	std::cout << "Tiles :" << std::endl;

	for (std::size_t i = 0; i <= 255; i++) {
		std::uint8_t	tile_id = 0;
		_memmap->read(tile_map + i, &tile_id, 1);

		std::uintptr_t	tile_addr = tile_data + (16 * tile_id);
		std::uint8_t	pixs[2] = {0};
		_memmap->read(tile_addr, pixs, 2);

		std::cout << "ID   : " << std::hex << (int)(tile_map + i) << " -> " << std::hex << (int)tile_id << std::endl;
		std::cout << "DATA : " << std::hex << (int)(tile_addr) << " -> " << std::hex << (int)pixs[0] << std::endl;
		std::cout << "       " << std::hex << (int)(tile_addr + 1) << " -> " << std::hex << (int)pixs[1] << std::endl;
		std::cout << "PIX  :" << std::endl;
		for (int bit = 7; bit >= 0; bit--)  {
			int pix = ((pixs[1] >> bit) & 0x1) | (((pixs[0] >> bit) & 0x1) << 1);
			std::cout << std::hex << (int)pix;
		}
		std::cout << std::endl;
	}
}
