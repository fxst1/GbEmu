#ifndef GBDATA_HH
# define GBDATA_HH
# include <cstdint>
# include <iostream>
# include <map>
# include <mutex>

# define OAM_SEARCH 0x02
# define PIXEL_TRANSFER 0x03
# define HBLANK 0x00
# define VBLANK 0x01

namespace	gameboy {

	struct	CartridgeType {

		typedef enum {
			ROM,
			MBC1,
			MBC2,
			MBC3,
			MBC5,
			MBC6,
			MBC7,
			MMM01,
			POCKET_CAMERA,
			BANDAI_TAMA5,
			HUC1,
			HUC3
		}	Version;

		Version		mbc_version;
		bool		ram;
		bool		battery;
		bool		timer;
		bool		rumble;
		std::string	str;
	};

	typedef struct	Const {
		static const std::uint8_t							NintendoLogo[48];
		static const std::map<std::uint16_t, std::string>	Publishers;
		static const std::map<std::uint8_t, CartridgeType>	CartTypes;
		static const std::uint8_t							DmgBoot[256];
	}	const_t;

	struct	CartridgeHeader {
		std::uint32_t	entrypoint;
		std::uint8_t	logo[0x30];

		union {
			char				fulltitle[0x10];
			struct {
				char			title[0xb];
				char			manufacturer[0x4];
				std::uint8_t	cgb;
			};
		};

		std::uint16_t	licence;
		std::uint8_t	sgb;
		std::uint8_t	carttype;
		std::uint8_t	romsize;
		std::uint8_t	ramsize;
		std::uint8_t	destcode;
		std::uint8_t	oldlicence;
		std::uint8_t	version;
		std::uint8_t	head_checksum;
		std::uint16_t	global_checksum;
	};

	static_assert(sizeof(struct CartridgeHeader) == 0x50, "Code relies on struct having no padding");

	std::ostream&		operator<<(std::ostream& os, struct gameboy::CartridgeHeader const &gbhead);

	struct	IORegisters {
		std::uint32_t*	portmode1;
		std::uint32_t*	portmode2;
		std::uint8_t*	sound;
		std::uint8_t*	waveram;
		std::uint8_t*	lcd;
		std::uint8_t*	vrambank;
		std::uint8_t*	disableboot;
		std::uint8_t*	hdma;
		std::uint8_t*	bcp_ocp;
		std::uint8_t*	wrambank;
	};

	struct				Oam {
		std::uint8_t	y;
		std::uint8_t	x;
		std::uint8_t	index;
		std::uint8_t	attr;
	};

	struct				Pixel {
		unsigned int	color;
		unsigned int	pal;
		unsigned int	priority;
		unsigned int	bg_priority;
		unsigned int	x;
		unsigned int	y;
	};


	struct		LCDRegisters {

		enum		LcdcType {
			lcd_enabled = 7,
			win_tilemap = 6,
			win_tilemap_enabled = 5,
			bgwin_tiledata = 4,
			bg_tilemap = 3,
			sprite_size = 2,
			sprite_enabled = 1,
			bgwin_priority = 0
		};

		std::uint8_t	lcdc; //ff40

		/*
			struct {
				unsigned	lcd_enabled:1;
				unsigned	win_tilemap:1;
				unsigned	win_tilemap_enabled:1;
				unsigned	bgwin_tiledata:1; //0=8800-97FF, 1=8000-8FFF
				unsigned	bg_tilemap:1;
				unsigned	sprite_size:1;
				unsigned	sprite_enabled:1;
				unsigned	bgwin_priority:1;
			};
		*/

		enum		StatType {
			unused_ff41_7 = 7,
			ly_lyc_int = 6,
			oam_int = 5,
			vblank_int = 4,
			hblank_int = 3,
			ly_lyc = 2,
			mode1 = 1,
			mode = 0
		};
		std::uint8_t	stat; //ff41

		/*
		struct {
			unsigned int	unused_ff41_7:1;
			unsigned int	ly_lyc_int:1;
			unsigned int	oam_int:1;
			unsigned int	vblank_int:1;
			unsigned int	hblank_int:1;
			unsigned int	ly_lyc:1;
			unsigned int	mode:2;
		};*/

		/***************************/

		std::uint8_t		scy; //ff42
		std::uint8_t		scx; //ff43
		std::uint8_t		ly; //ff44
		std::uint8_t		lyc; //ff45


		std::uint8_t		dma_start_addr; //ff46

		union {
			std::uint8_t	bgp;	//ff47
			/*
			struct {
				unsigned int	bgp_grey0:2;
				unsigned int	bgp_grey1:2;
				unsigned int	bgp_grey2:2;
				unsigned int	bgp_grey3:2;
			};
			*/
		};
		std::uint8_t		obp0;	//ff48
		std::uint8_t		obp1;	//ff49


		std::uint8_t		wy; //ff4A
		std::uint8_t		wx; //ff4B

	};

	struct LCDRegistersColor: LCDRegisters {
		std::uint8_t		vram_bank; //ff4F

		/***************************/

		union {
			std::uint8_t	bcps; //ff68
			struct {
				unsigned int	bcps_index:6;
				unsigned int	bcps_autoinc:1;
			};
		};
		std::uint8_t	bcp_data; //ff69

		std::uint8_t	ocps; //ff6A
		std::uint8_t	ocpd; //ff6B

		/***************************/

		union {
			std::uint16_t		hdma_src; //ff51 - ff52
			struct {
				std::uint8_t	hdma_src_h;
				std::uint8_t	hdma_src_l;
			};
		};

		union {
			std::uint16_t		hdma_dst; //ff53 - ff54
			struct {
				std::uint8_t	hdma_dst_h;
				std::uint8_t	hdma_dst_l;
			};
		};

		union {
			std::uint8_t		hdma_ctrl; //FF55
			struct {
				unsigned int	hdma_ctrl_mode:1;
				unsigned int	hdma_ctrl_unused:6;
			};
		};
	};

	union TileData{
		std::uint16_t		line;
		struct {
			std::uint8_t	line1;
			std::uint8_t	line2;
		};
	};

	union BgMapAttrs {
		std::uint8_t		attrs;
		struct {
			std::uint8_t	bg2oam_priority:1;
			std::uint8_t	vflip:1;
			std::uint8_t	hflip:1;
			std::uint8_t	unused:1;
			std::uint8_t	tile_bank:1;
			std::uint8_t	bg_pal:3;
		};
	};

	union OAM {
		std::uint8_t		attrs;
		struct {
			std::uint8_t	y;
			std::uint8_t	x;
			std::uint8_t	tile_num;
			BgMapAttrs		flags;
		};
	};

	/*					struct {
							unsigned int	zerof:1;
							unsigned int	addf:1;
							unsigned int	hcarryf:1;
							unsigned int	carryf:1;
							unsigned int	filled:4;
						};
					};

	*/

	typedef struct {

		enum Flags {
			zero = 7,
			add = 6,
			half_carry = 5,
			carry = 4
		};

		union {
			std::uint16_t		af;
			struct {
				std::uint8_t	f;
				std::uint8_t	a;
			};
		};

		union {
			std::uint16_t		bc;
			struct {
				std::uint8_t	c;
				std::uint8_t	b;
			};
		};

		union {
			std::uint16_t		de;
			struct {
				std::uint8_t	e;
				std::uint8_t	d;
			};
		};

		union {
			std::uint16_t		hl;
			struct {
				std::uint8_t	l;
				std::uint8_t	h;
			};
		};

		std::uint16_t			sp;
		std::uint16_t			pc;

		bool					ime;
	}							CpuRegisters;
};
#endif
