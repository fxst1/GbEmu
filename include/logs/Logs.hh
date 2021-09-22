#pragma once
# include <string>
# include <fstream>
# include <ctime>
# include <sstream>
# include <iostream>

namespace	logs {

	class	ILogger {
		public:
			ILogger(void);
			ILogger(std::string const & name);
			ILogger(logs::ILogger const & log);
			virtual ~ILogger(void) {}

			logs::ILogger&			operator=(logs::ILogger const &log);

			std::string				name(void) const;
			virtual std::string		infoHead(void) const;
			virtual std::ostream&	output(bool show_info);
			virtual std::ostream&	output(void) = 0;

			virtual std::istream&	input(std::string const &prompt);
			virtual std::istream&	input(void) = 0;

		protected:
			std::string				_name;
	};

	class	Logger: public logs::ILogger {
		public:
			Logger(void);
			Logger(logs::Logger const &log);
			Logger(std::string const &name);
			Logger(std::string const &name, std::ostream &os, std::istream &is);
			Logger(std::string const &name, std::istream &is);
			Logger(std::string const &name, std::ostream &os);
			virtual ~Logger(void);

			logs::Logger&			operator=(logs::Logger const &log);

			virtual std::ostream&	output(void);
			virtual std::istream&	input(void);

		protected:
			std::ostream*		_output;
			std::istream*		_input;
	};

	class	FileLogger: public logs::ILogger {
		public:
			FileLogger(std::string const &name, std::string const &path);
			~FileLogger(void);

			virtual std::ostream&	output(void);
			virtual std::istream&	input(void);

		protected:
			std::string			_path;
			std::fstream		_file;
	};
};
