#include "debug/Debugger.hh"
void	debug::Debugger::goNext(std::vector<std::string> const & args) {
	_states->start();
	_pass = false;
	(void)args;
}
void	debug::Debugger::goCont(std::vector<std::string> const & args) {
	::memcpy(this->_states->programCounter(), &_pc_brk, _addr_size);
	_states->start();
	_pass = true;
	(void)args;
}
void	debug::Debugger::file(std::vector<std::string> const & args) {
	this->_states->log("cmd-info") << "* Loading file '" << args.at(1) << "'" << std::endl;
	this->load(args[1]);
}
void	debug::Debugger::run(std::vector<std::string> const & args) {
	this->_states->log("cmd-info") << "* Starting *" << std::endl;
	(void)args;
}


bool	debug::Debugger::breakpointLookup(void) {
	if (_pc_brk > 0 && ::memcmp(_states->programCounter(), &_pc_brk, _addr_size) == 0) {
		return (false);
	}

	for (std::uintptr_t const & addr : this->_breakpoints) {
		if (::memcmp(this->_states->programCounter(), &addr, _addr_size) == 0) {

			std::ostream&	os = _states->log("cmd-output");
			os << "Breakpoint reached: ";

			this->showAddress(os, addr) << std::endl;
			::memcpy(&_pc_brk, this->_states->programCounter(), _addr_size);

			_states->stop();
			return (true);
		}
	}
	return (false);
}

void	debug::Debugger::breakpoint(std::vector<std::string> const &args) {
	try {

		if (args.size() <= 1 || args[1] == "list") {
			if (_breakpoints.empty()) {
				this->_states->log("cmd-info", false) << "No Breakpoints" << std::endl;
			} else {
				this->_states->log("cmd-info", false) << "Breakpoints:" << std::endl;
				for (const std::uintptr_t &addr: _breakpoints) {
					this->_states->log("cmd-info", false) << "* " << std::hex << (int)addr << std::endl;
				}
			}
		} else if (args[1] == "+" || args[1] == "-") {
			std::uintptr_t		addr;
			std::stringstream	sstr;
			sstr << args[2];
			sstr >> std::hex >> std::setw(2) >> addr;

			if (args[1] == "+") this->addBreakpoint(addr);
			else this->delBreakpoint(addr);

		} else {
			this->_states->log("cmd-error", false) << "Usage : brk {[+-] <address>}" << std::endl;
		}

	} catch (std::out_of_range &oor) {
		this->_states->log("cmd-error", false) << "brk: Argument missing" << std::endl;
		this->_states->log("cmd-error", false) << "Usage : brk {[+-] <address>}" << std::endl;
	}
}


void 	debug::Debugger::addBreakpoint(std::uintptr_t addr) {
	this->_breakpoints.insert(addr);

	std::ostream&	os = _states->log("cmd-output");
	os << "Breakpoint added at: ";
	this->showAddress(os, addr) << std::endl;
}

void 	debug::Debugger::delBreakpoint(std::uintptr_t addr) {
	this->_breakpoints.erase(addr);

	std::ostream&	os = _states->log("cmd-output");
	os << "Breakpoint removed at: ";
	this->showAddress(os, addr) << std::endl;
}

void	debug::Debugger::accessMemory(std::vector<std::string> const &args) {
	try {
		int					addr = 0;
		std::size_t			len = 0;
		if (args.size() <= 3) {
			std::stringstream	sstr;
			sstr << args[2];
			sstr >> std::hex >> addr;
		}

		std::stringstream	sstr;
		sstr << args[1];
		sstr >> std::hex >> std::setw(4) >> addr;

		std::cout << "* Memory: @"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< ((int)addr);
		if (len > 0) {
			std::cout << " sizeof " << memunit::Memory::prettySize(len);
		}
		std::cout << std::endl;
		std::cout << "* Mem Dump" << std::endl;
		_memmap->printMemory(std::cout, addr, (len == 0) ? 32 : len);

	} catch (std::out_of_range &oor) {
		this->_states->log("cmd-error", false) << "mem: Argument missing" << std::endl;
		this->_states->log("cmd-error", false) << "Usage : mem <address> <size>" << std::endl;
	}
}
/*
void	debug::Debugger::accessCode(std::vector<std::string> const &args) {
	try {
		if (args.size() == 2) {
			std::string		reg_name = args[1];

			std::transform(reg_name.begin(), reg_name.end(), reg_name.begin(),
    		[](unsigned char c){ return std::tolower(c); });

			bool			isreg8 = true;
			std::string		regn = "";
			std::uint8_t	reg8 = 0;
			std::uint16_t	reg16 = 0;
			if (reg_name == "a") { regn = "A"; reg8 = _cpuregs->a; }
			else if (reg_name == "b") { regn = "B"; reg8 = _cpuregs->b; }
			else if (reg_name == "c") { regn = "C"; reg8 = _cpuregs->c; }
			else if (reg_name == "d") { regn = "D"; reg8 = _cpuregs->d; }
			else if (reg_name == "e") { regn = "E"; reg8 = _cpuregs->e; }
			else if (reg_name == "h") { regn = "H"; reg8 = _cpuregs->h; }
			else if (reg_name == "l") { regn = "L"; reg8 = _cpuregs->l; }
			else if (reg_name == "f") { regn = "F"; reg8 = _cpuregs->f; }
			else {
				isreg8 = false;
				if (reg_name == "bc") { regn = "BC"; reg16 = _cpuregs->bc; }
				else if (reg_name == "de") { regn = "DE"; reg16 = _cpuregs->de; }
				else if (reg_name == "hl") { regn = "HL"; reg16 = _cpuregs->hl; }
				else if (reg_name == "sp") { regn = "SP"; reg16 = _cpuregs->sp; }
				else if (reg_name == "pc") { regn = "PC"; reg16 = _cpuregs->pc; }
			}

			if (regn == "SP") //Stack
			{
				std::cout << "Stack Trace: " << std::endl;
				for (std::size_t i = 0; (reg16 + i) < 0xffff && i < 16; i++) {
					std::uint8_t	val = 0;
					_memmap->read(reg16 + i, &val, sizeof(val));
					std::cout << "* @"
						<< std::hex << std::setw(4) << std::setfill('0')
						<< ((int)reg16 + i)
						<< " -> $"
						<< std::hex << std::setw(2) << std::setfill('0')
						<< (int)val
						<< std::endl;
				}
			} else if (regn == "PC") {

			} else if (regn != ""){
				std::uint8_t	val = 0;
				_memmap->read(isreg8 ? reg8 : reg16, &val, sizeof(val));
				std::cout << "* " << regn << " : @"
				<< std::hex << std::setw(4) << std::setfill('0')
				<< ((int)(isreg8 ? reg8 : reg16))
				<< " -> $"
				<< std::hex << std::setw(2) << std::setfill('0')
				<< (int)val
				<< std::endl;
			}
		} else {

			std::uint8_t	val = 0;

			_memmap->read(_cpuregs->bc, &val, sizeof(val));
			std::cout << "* BC : @"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< (int)_cpuregs->bc
			<< " -> $"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)val
			<< std::endl;

			_memmap->read(_cpuregs->de, &val, sizeof(val));
			std::cout << "* DE : @"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< (int)_cpuregs->de
			<< " -> $"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)val
			<< std::endl;

			_memmap->read(_cpuregs->hl, &val, sizeof(val));
			std::cout << "* HL : @"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< (int)_cpuregs->hl
			<< " -> $"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)val
			<< std::endl;

			_memmap->read(_cpuregs->sp, &val, sizeof(val));
			std::cout << "* SP : $"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< (int)_cpuregs->sp
			<< " -> $"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)val
			<< std::endl;

			_memmap->read(_cpuregs->pc, &val, sizeof(val));
			std::cout << "* PC : $"
			<< std::hex << std::setw(4) << std::setfill('0')
			<< (int)_cpuregs->pc
			<< " -> $"
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)val
			<< std::endl;
		}
	} catch (std::out_of_range &oor) {
		std::cerr << "cpu: Argument missing" << std::endl;
		std::cout << "Usage : cpu {<register name>}" << std::endl;
	}
}*/
/*
void	debug::Debugger::accessPpu(std::vector<std::string> const &args){
	(void)args;
}
*/
