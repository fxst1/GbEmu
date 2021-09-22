#pragma once
# include <mutex>
# include <map>

# include "logs/Logs.hh"
# include "memunit/Memory.hh"

namespace emu {

	class	States {

		private:

			bool									_onboot;
			bool									_running;
			std::recursive_mutex					_mutex;
			std::map<std::string, logs::ILogger*>	_logs;

		public:

			States(void);
			virtual ~States(void);

			memunit::Memory*		_memmap;

			void			setLog(logs::ILogger &log);
			void			unsetLog(std::string const &name);
			std::ostream&	log(std::string const &name, bool show_info = true);
			std::istream&	ilog(std::string const &name, std::string const & prompt);

			bool			isBooting(void) const;
			void			setBooting(bool boot);

			bool			isRunning(void) const;
			void			start(void);
			void			stop(void);

			void			lock(void);
			void			unlock(void);

			virtual std::uint8_t*	programCounter(void) const = 0;
			virtual std::uint8_t*	stackPointer(void) const = 0;
	};

};
