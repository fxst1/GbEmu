#include "debug/Loggers.hh"

debug::StateLogger::StateLogger(gameboy::States & states, std::string const name,
	std::size_t n_cols, std::size_t n_rows, std::size_t y, std::size_t x):
	logs::TermLogger(name, n_cols, n_rows, y, x),
	_states(states)
{}
debug::StateLogger::~StateLogger()
{}
