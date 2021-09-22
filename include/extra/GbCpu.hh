#ifndef GB_CPU_HH
# define GB_CPU_HH
# include <cstdint>
# include "opcode/OpcodeBuilder.hh"
# include "timing/Timing.hh"
# include "extra/GbData.hh"
# include "extra/GbStates.hh"

namespace	gameboy {

	class	GbCpu: public timing::Tick {

		public:

			GbCpu(gameboy::States& stat);
			~GbCpu(void);

			void						setMemory(memunit::Memory &memory);
			void						tick(void);
			opcode::InstructionSet		opcodes(void) const;
			gameboy::CpuRegisters*		regs(void);

		protected:
			void 						showStack(void) const;
			void 						showRegs(void) const;

			void 						fetchNext(void);
			void 						fetchNext(opcode::Opcode const & instruction);

		private:

			gameboy::States&			_stat;
			gameboy::CpuRegisters		_regs;
			memunit::Memory*			_memmap;
			opcode::InstructionSet		_opcodes;
			int							_cycles_count;

			int							nTickPerFrame(void) const;
			void						prepare(void);
	};
};

#endif
