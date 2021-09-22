#include "logs/Logs.hh"

logs::ILogger::ILogger(void): _name("undefined logger name") {}
logs::ILogger::ILogger(std::string const &name): _name(name) {}
logs::ILogger::ILogger(logs::ILogger const &log): _name(log._name) {}
logs::ILogger&		logs::ILogger::operator=(logs::ILogger const &log) {
	if (this != &log) {
		this->_name = log._name;
	}
	return (*this);
}
std::string		logs::ILogger::name(void) const {
	return (this->_name);
}

std::ostream&	logs::ILogger::output(bool show_info) {
	if (show_info)
		return (this->output() << this->infoHead());
	return (this->output());
}
std::istream&	logs::ILogger::input(std::string const &prompt) {
	this->output() << this->name() << prompt;
	return (this->input());
}

std::string		logs::ILogger::infoHead(void) const {

	std::stringstream	sstr;
	time_t				rawtime;
	struct tm*			timeinfo;
	char				buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);

	sstr << "[" << buffer << "] - " << this->name() << ": ";
	return (sstr.str());
}

/************************/

logs::Logger::Logger(void): logs::ILogger(),
_output(nullptr), _input(nullptr) {}
logs::Logger::Logger(std::string const &name): logs::ILogger(name),
_output(nullptr), _input(nullptr) {}
logs::Logger::Logger(std::string const &name, std::ostream &os): logs::ILogger(name),
_output(&os), _input(nullptr) {}
logs::Logger::Logger(std::string const &name, std::istream &is): logs::ILogger(name),
_output(nullptr), _input(&is) {}
logs::Logger::Logger(std::string const &name, std::ostream &os, std::istream &is): logs::ILogger(name),
_output(&os), _input(&is) {}
logs::Logger::Logger(logs::Logger const &log): logs::ILogger(log),
_output(log._output), _input(log._input) {}
logs::Logger::~Logger(void) {}

std::ostream&	logs::Logger::output(void) {
	return (*this->_output);
}
std::istream&	logs::Logger::input(void) {
	return (*this->_input);
}

/********************************************/

logs::FileLogger::FileLogger(std::string const &name, std::string const &path):
	logs::ILogger(name),
	_path(path), _file(path, std::ios::out | std::ios::trunc)
{}

logs::FileLogger::~FileLogger(void) {
	if (_file.is_open())
		_file.close();
}

std::ostream&	logs::FileLogger::output(void) {
	return (this->_file);
}
std::istream&	logs::FileLogger::input(void) {
	return (this->_file);
}
