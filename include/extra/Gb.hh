#ifndef GB_HH
# define GB_HH

# include <thread>

# include "extra/GbCpu.hh"
# include "extra/GbPpu.hh"
# include "extra/GbData.hh"
# include "extra/GbStates.hh"
# include "extra/GbMemUnit.hh"

# include "timing/Timing.hh"

namespace	gameboy {
	class	Gameboy {

		public:

			Gameboy(gameboy::States& states);
			~Gameboy(void);

			void		setDisplayDriver(gameboy::GbPpu::LCDInterface * disp);

			void		onAfterCycle(void);

			void		load(std::string const & filename);
			void		loadBoot(std::string const & filename);
			void		tick(void);
			void		start(void);
			void		stop(void);
			void		mainloop(void);

			gameboy::States&		states(void);
			timing::Clock&			clock(void);
			opcode::InstructionSet	instructionSet(void);
			memunit::MemoryMap*		memoryMap(void);
			gameboy::CpuRegisters* 	cpuRegisters(void);
			gameboy::GbPpu* 		ppu(void);

		private:

			timing::HertzClock		_clock;
			gameboy::States&		_stat;
			gameboy::GbPpu			_ppu;
			gameboy::GbCpu			_cpu;
			gameboy::GbGameLoader	_loader;
			gameboy::GbMemMap		_memmap;
	};
};

#endif
