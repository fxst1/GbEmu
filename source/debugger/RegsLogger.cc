#include "debug/Loggers.hh"

debug::RegisterLogger::RegisterLogger(gameboy::States & states):
	debug::StateLogger(states, "register", 30, 30, 0, 0)
{}
debug::RegisterLogger::~RegisterLogger()
{}

void		debug::RegisterLogger::showRegister(std::string const &reg_name, std::uint8_t reg_val) {

	std::uint8_t	val;
	_states._memmap->read(reg_val, &val, sizeof(val));

	this->output()
		<< reg_name << " : $" << std::hex << std::setfill('0') << std::setw(2) << (int)reg_val
			<< " -> [$" << std::hex << std::setfill('0') << std::setw(4) << (int)val << "]" << std::endl;
	this->print();
}

void		debug::RegisterLogger::tick(void) {
	::clear();
	this->showRegister("A", _states._cpuregs->a);
	this->showRegister("F", _states._cpuregs->f);
	logs::TermLogger::tick();
}
