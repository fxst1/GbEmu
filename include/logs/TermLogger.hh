#pragma once
# include "logs/Logs.hh"
# include "timing/Timing.hh"
# include "ncurses.h"

namespace	logs {

	class	TermLogger: public logs::ILogger, public timing::Tick {
		public:
			TermLogger(std::string const & name,
				std::size_t n_cols, std::size_t n_rows,
				std::size_t y, std::size_t x);
			~TermLogger(void);

			virtual void			tick(void);
			virtual std::ostream&	output(void);
			virtual std::istream&	input(void);

			std::string				flush(void);
			std::string				print(void);

		protected:
			std::stringstream		_stream;
			::WINDOW*				_win;
			std::size_t				_n_cols;
			std::size_t				_n_rows;

			std::size_t				_x;
			std::size_t				_y;
			std::size_t				_start_y;
	};

};
