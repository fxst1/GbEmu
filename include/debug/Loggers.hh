#pragma once
# include "logs/TermLogger.hh"
# include "timing/Timing.hh"
# include "extra/GbMemUnit.hh"
# include "extra/GbStates.hh"

namespace	debug {

	class	StateLogger:  public logs::TermLogger {
		public:
			StateLogger(gameboy::States & states, std::string const name,
				std::size_t n_cols, std::size_t n_rows, std::size_t y, std::size_t x);
			~StateLogger(void);

		protected:
			gameboy::States&		_states;
	};

	class	RegisterLogger: public debug::StateLogger {
		public:
			RegisterLogger(gameboy::States & states);
			~RegisterLogger(void);

			void		showRegister(std::string const &reg_name, std::uint8_t reg_val);
			void		tick(void);
	};

	class	CodeLogger: public debug::StateLogger {
		public:
			CodeLogger(gameboy::States & states);
			~CodeLogger(void);

			void		display(void);
			void		tick(void);
	};

	class	MainLogger: public debug::StateLogger {
		public:
			MainLogger(gameboy::States & states);
			~MainLogger(void);

			void		display(void);
			void		tick(void);
	};

};
