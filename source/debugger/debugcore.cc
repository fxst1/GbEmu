#include "debug/Debugger.hh"

debug::Debugger::Debugger(std::size_t addr_size, std::size_t data_size):
	_addr_size(addr_size), _data_size(data_size),
	_cmd(), _cmd_name(), _cmd_args(),
	_breakpoints(), _pc_brk(),
	_exit(false),
	_funcs(), _watcs(),
	_history(), _history_log(nullptr)
{ this->init(); }

debug::Debugger::~Debugger(void){
	if (this->_history_log) delete this->_history_log;
}

void 	debug::Debugger::openHistory(std::string const &filename) {
	this->_history_log = new logs::FileLogger("history", filename);
}

void 	debug::Debugger::setFunction(std::string const &fname, debug::Debugger::DebugFunction func) {
	_funcs[fname] = func;
}
void 	debug::Debugger::setWatcher(std::string const &wname, debug::Debugger::WatcherFunction func) {
	_watcs[wname] = func;
}

void 	debug::Debugger::init(void) {
	this->setWatcher("brkpts", std::bind(&Debugger::breakpointLookup, this));

	//_funcs["log"] = std::bind(&Debugger::useLog, this, std::placeholders::_1);

	_funcs["file"] = std::bind(&Debugger::file, this, std::placeholders::_1);
	_funcs["run"] = std::bind(&Debugger::run, this, std::placeholders::_1);
	_funcs["pass"] = std::bind(&Debugger::goCont, this, std::placeholders::_1);
	_funcs["next"] = std::bind(&Debugger::goNext, this, std::placeholders::_1);

	_funcs["brk"] = std::bind(&Debugger::breakpoint, this, std::placeholders::_1);
	_funcs["mem"] = std::bind(&Debugger::accessMemory, this, std::placeholders::_1);

	_funcs["exit"] = [&](std::vector<std::string> const & args) {
		_states->stop();
		_exit = true;
		this->stop();
		(void)args;
	};

	_funcs["help"] = [&](std::vector<std::string> const & args) {
		std::cout << "HELP"<< std::endl;
		(void)args;
	};
}

emu::States&	debug::Debugger::states(void) { return (*this->_states); }
void	debug::Debugger::set(emu::States& statements) {
	this->_states = &statements;
}

std::ostream&	debug::Debugger::showAddress(std::ostream &os, std::uintptr_t addr) const {
	os << "@" << std::hex << std::setw(_addr_size * 2) << std::setfill('0') << (int)addr;
	return (os);
}
std::ostream&	debug::Debugger::showValue(std::ostream &os, std::uint32_t value) const {
	os << "$" << std::hex << std::setw(_data_size * 2) << std::setfill('0') << (int)value;
	return (os);
}

void	debug::Debugger::set(memunit::Memory* memmap){
	_memmap = memmap;
}
void	debug::Debugger::set(opcode::InstructionSet& opcodes) {
	_opcodes = opcodes;
}

void		debug::Debugger::stop(void) {
}

void		debug::Debugger::start(void) {
	_pass = false;
	while (!_exit && this->catchEvent()) {

		//this->dispatchEvent();

		this->_states->lock();

		if (_pass && ::memcmp(_states->programCounter(), &_pc_brk, _addr_size) != 0) {
			_states->start();
		}

		for (auto it = this->_watcs.begin(); it != this->_watcs.end(); it++) {
			if ((it->second)() == true) break ;
		}
		this->_states->unlock();

		this->tick();

		this->_states->lock();
		if (_pass && ::memcmp(_states->programCounter(), &_pc_brk, _addr_size) != 0) {
			std::cout << "* Halt Program" << std::endl;
			::memcpy(&_pc_brk, _states->programCounter(), _addr_size);
			_states->stop();
			_pass = false;
		}
		this->_states->unlock();
	}
}

void		debug::Debugger::dispatchEvent(void) {}
bool		debug::Debugger::catchEvent(void) { return (true); }

void	debug::Debugger::tick(void) {
	this->_states->log("cmd-output", false) << "debugger> ";
	this->commandLine();
	this->parseCommand();
	if (!_cmd_args.empty()) {
		auto it = _funcs.find(_cmd_name);
		if (it == _funcs.end()) {
			std::cerr << "Command `"<<_cmd_name<<"` not found" << std::endl;
		} else {
			this->_states->lock();
			try {
				(it->second)(_cmd_args);

				if (!_cmd.empty()) {
					if (this->_history_log)
						this->_history_log->output() << _cmd << std::endl;
					this->_history.push_front(_cmd);
				}

			} catch (std::out_of_range &oor) {
				std::cout << "Argument out of range: " << oor.what() << std::endl;
			} catch (std::exception& e) {
				std::cout << "An exception occured: " << e.what() << std::endl;
			}
			this->_states->unlock();
		}
	}
}

void	debug::Debugger::commandLine(void) {
	std::getline(this->_states->ilog("cmd-input", ">"), _cmd);
}

void	debug::Debugger::parseCommand(void) {

	std::vector<std::string> 	splited;
	std::string::size_type		prev_pos = 0;
	std::string::size_type		pos = 0;

	while ((pos = _cmd.find(' ', pos)) != std::string::npos) {
		std::string	s = _cmd.substr(prev_pos, pos - prev_pos);
		if (!s.empty()) {
			splited.push_back(s);
		}
		prev_pos = ++pos;
	}

	std::string	s = _cmd.substr(prev_pos, pos - prev_pos);

	if (!s.empty())
		splited.push_back( _cmd.substr(prev_pos, pos - prev_pos) );

	if (!splited.empty()) {
		_cmd_name = splited[0];
		_cmd_args = splited;
	}
}
