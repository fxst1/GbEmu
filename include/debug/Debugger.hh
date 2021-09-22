#ifndef DEBUG_HH
# define DEBUG_HH
# include <iterator>
# include <algorithm>
# include <string>
# include <vector>
# include <set>
# include <map>

# include <unistd.h>
# include <stdio.h>
# include <curses.h>

# include "opcode/OpcodeBuilder.hh"
# include "memunit/Memory.hh"
# include "timing/Timing.hh"
# include "emu/States.hh"
# include "logs/SocketLogger.hh"

namespace debug {
	class	Debugger: public timing::Tick {
		public:

			typedef std::function<void(std::vector<std::string> const &)>	DebugFunction;
			typedef std::function<bool(void)> 								WatcherFunction;

			Debugger(std::size_t addr_size, std::size_t data_size);
			virtual ~Debugger(void);

			void 	setFunction(std::string const &fname, DebugFunction func);
			void 	setWatcher(std::string const &wname, WatcherFunction func);

			void	openHistory(std::string const &filename);

			void	set(emu::States& statements);
			void	set(opcode::InstructionSet& opcodes);
			void	set(memunit::Memory* memmap);

			emu::States&	states(void);


			virtual void	init(void);
			virtual void	start(void);
			virtual void	stop(void);
			virtual void	tick(void);

			virtual void	dispatchEvent(void);
			virtual bool	catchEvent(void);

			void	commandLine(void);

			void			goCont(std::vector<std::string> const &args);
			void			goNext(std::vector<std::string> const &args);
			void			file(std::vector<std::string> const &args);
			virtual void	load(std::string const &filename) = 0;
			virtual void	run(std::vector<std::string> const &args);

			void			useLog(std::vector<std::string> const &args);


			bool	breakpointLookup(void);
			void	breakpoint(std::vector<std::string> const &args);
			void	addBreakpoint(std::uintptr_t addr);
			void	delBreakpoint(std::uintptr_t addr);

			void	accessMemory(std::vector<std::string> const &args);


			std::ostream&	showAddress(std::ostream &os, std::uintptr_t addr) const;
			std::ostream&	showValue(std::ostream &os, std::uint32_t val) const;

		private:

			std::size_t						_addr_size;
			std::size_t						_data_size;

			memunit::Memory*				_memmap;
			opcode::InstructionSet			_opcodes;
			emu::States*					_states;

			std::string						_cmd;
			std::string						_cmd_name;
			std::vector<std::string>		_cmd_args;
			std::set<std::uintptr_t>		_breakpoints;
			std::uintptr_t					_pc_brk;
			bool							_pass;

			bool									_exit;
			std::map<std::string, DebugFunction>	_funcs;
			std::map<std::string, WatcherFunction>	_watcs;

			std::list<std::string>					_history;
			logs::FileLogger*						_history_log;

			void							parseCommand(void);
	};

};

#endif
