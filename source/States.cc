#include "emu/States.hh"

emu::States::States(void):
	_onboot(false),
	_running(false),
	_mutex(),
	_logs()
{}

emu::States::~States(void)
{}

/******************************************/

void		emu::States::lock(void) {
	this->_mutex.lock();
}
void		emu::States::unlock(void) {
	this->_mutex.unlock();
}

/******************************************/

void		emu::States::setLog(logs::ILogger &log) {
	this->_logs[log.name()] = &log;

}
void		emu::States::unsetLog(std::string const &name) {
	this->_logs.erase(name);
}
std::ostream&	emu::States::log(std::string const &name, bool show_info) {
	logs::ILogger*	l = this->_logs[name];
	if (l)
		return (l->output(show_info));
	return (std::cout);
}
std::istream&	emu::States::ilog(std::string const &name, std::string const &in) {
	logs::ILogger*	l = this->_logs[name];
	if (l)
		return (l->input(in));
	return (std::cin);
}

/******************************************/

bool		emu::States::isBooting(void) const {
	return (this->_onboot);
}
void		emu::States::setBooting(bool boot) {
	this->_onboot = boot;
}

/******************************************/

bool		emu::States::isRunning(void) const {
	return (this->_running);
}
void		emu::States::start(void) {
	std::cout << "* Start request" << std::endl;
	this->_running = true;
}
void		emu::States::stop(void) {
	std::cout << "* Stop request" << std::endl;
	this->_running = false;
}
