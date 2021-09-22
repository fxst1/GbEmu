#ifndef GB_MEMUNIT_HH
# define GB_MEMUNIT_HH
# include "memunit/MemUnit.hh"
# include "extra/GbData.hh"
# include <fstream>
# include <map>

namespace	gameboy {

	class	MemoryBank: public memunit::Memory {

		protected:

			std::map<std::uint8_t, std::uint8_t*>	_banks;
			std::uint8_t							_bank;
			std::uintptr_t							_base;
			std::size_t								_size;
			bool									_is_rom;

			void					init(std::uint8_t start, std::uint8_t end, std::uintptr_t base, std::size_t size);

		public:

			MemoryBank(bool is_rom = false);
			MemoryBank(std::uint8_t start, std::uint8_t end, std::uintptr_t base, std::size_t size);
			MemoryBank(std::size_t nbanks, std::uintptr_t base, std::size_t size);
			virtual ~MemoryBank(void);

			void					setBank(std::uint8_t bank);
			std::uint8_t			getBank(void) const;

			virtual std::size_t		allocSize(void) const;
			virtual std::uintptr_t	base(void) const;

			virtual bool			canRead(void) const;
			virtual bool			canWrite(void) const;
			virtual bool			canExecute(void) const;

			virtual std::uint8_t*	data(void) const;
	};

	class	MBC: public memunit::MemoryMap {

		public:

			typedef enum {
				RomBanking = 0,
				RamBanking = 1
			}	Mode;

			MBC(memunit::Memory & m);
			virtual ~MBC(void);

			memunit::Memory*	lorom(void);
			memunit::Memory*	hirom(void);
			memunit::Memory*	externRam(void);

		protected:

			Mode				_mode;
			bool				_ram_enabled;

		private:

			class	LowRom: public gameboy::MemoryBank {
				private:
					MBC&				_mbc;
					memunit::Memory&	_rom;

				public:
					LowRom(MBC& mbc, memunit::Memory & m);
					~LowRom(void);
					std::size_t		write(std::uintptr_t at, const std::uint8_t *src, std::size_t buflen);
			};

			class	HiRom: public gameboy::MemoryBank {
				private:
					MBC&		_mbc;

				public:
					HiRom(MBC& mbc);
					~HiRom(void);
					std::size_t		write(std::uintptr_t at, const std::uint8_t *src, std::size_t buflen);
			};

			LowRom					_lowrom;
			HiRom					_highrom;
			MemoryBank				_ram;
	};

	class	GbGameLoader {
		private:

			std::fstream			_input;
			struct CartridgeHeader	_head;
			std::string				_filename;
			std::size_t				_filesize;
			std::uint8_t*			_filedata;

		public:

			GbGameLoader(void);
			GbGameLoader(std::string const &filename);
			~GbGameLoader(void);

			std::fstream&	file(void);
			void			load(std::string const &filename);
			void			loadBoot(std::string const &filename);

			std::string		filename(void) const;
			std::size_t		filesize(void) const;
			std::uint8_t*	filebuf(void) const;

			std::string		getGameTitle(void) const;
			std::string		getPublisher(void) const;
			std::string		getManufacturer(void) const;

			std::size_t		getOldLicence(void) const;
			std::size_t		getVersion(void) const;
			std::size_t		getHeaderChecksum(void) const;
			std::size_t		getGlobalChecksum(void) const;

			std::uint8_t	computeHeaderChecksum(void) const;

			bool			check(void) const;
			bool			checkHeader(void) const;
			bool			checkLogo(void) const;

			bool			needColorSupport(void) const;
			bool			needSuperSupport(void) const;

			gameboy::CartridgeType	getCartridgeType(void) const;
			std::size_t				getCartridgeTypeFlag(void) const;

			gameboy::MBC*	getBankController(void) const;

			std::size_t		getRomFlag(void) const;
			std::size_t		getRomSize(void) const;

			std::size_t		getRamFlag(void) const;
			std::size_t		getRamSize(void) const;

			std::size_t		onWrite(memunit::Memory const & m, const std::uint8_t *src, std::size_t buflen);
			std::size_t		onRead(memunit::Memory const & m, std::uint8_t *dst, std::size_t buflen);
	};

	class	GbMemMap: public memunit::MemoryMap {
		public:

			GbMemMap(void);
			~GbMemMap(void);

			void	init(gameboy::GbGameLoader const &loader);
			void	reset(void);

			void	loadRomData(std::uint8_t* filedata, std::size_t filesize);

			std::size_t		write(std::uintptr_t at, const std::uint8_t *dst, std::size_t buflen);
			std::size_t		read(std::uintptr_t at, std::uint8_t *dst, std::size_t buflen);

			memunit::Memory*			getVram(void);

		private:

			bool					_onboot;
			gameboy::MBC*			_mbc;
			memunit::Memory*		_bootrom;
			memunit::Memory*		_lorom;
			memunit::Memory*		_hirom;
			memunit::Memory*		_extern_ram;
			memunit::Memory*		_vram;
			memunit::Memory*		_wram;
			memunit::Memory*		_wram_switchable;
			memunit::Memory*		_echoram;
			memunit::Block			_oam;
			memunit::Block			_not_usable;
			memunit::Block			_io_registers;
			memunit::Block			_hram;
			memunit::Block			_interrupts;
	};

	std::ostream&		operator<<(std::ostream& os, gameboy::GbGameLoader const &loader);
};

#endif
