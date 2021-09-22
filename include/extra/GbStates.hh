#ifndef GBDATA_STATES_HH
# define GBDATA_STATES_HH
# include <cstdint>
# include <iostream>
# include <map>
# include <mutex>

# include "emu/States.hh"
# include "logs/Logs.hh"
# include "extra/GbData.hh"
# include "extra/GbMemUnit.hh"

namespace	gameboy {

	class	States: public emu::States {
		public:
			States(void);
			~States(void);

			gameboy::CpuRegisters*	_cpuregs;

			std::uint8_t*	programCounter(void) const;
			std::uint8_t*	stackPointer(void) const;
	};
};

#endif
