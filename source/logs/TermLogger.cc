#include "logs/TermLogger.hh"

logs::TermLogger::TermLogger(std::string const &name, std::size_t n_cols, std::size_t n_rows, std::size_t y, std::size_t x):
	logs::ILogger(name),
	_win(nullptr),
	_n_cols(n_cols),
	_n_rows(n_rows),
	_x(x),
	_y(y)
{
	_win = ::subwin(stdscr, _n_cols, _n_rows, x, y);
	::box(_win, ACS_VLINE, ACS_HLINE);
}

logs::TermLogger::~TermLogger(void) {
	if (_win) free(_win);
}

std::istream&	logs::TermLogger::input(void) {
	return (this->_stream);
}
std::ostream&	logs::TermLogger::output(void) {
	return (this->_stream);
}

void			logs::TermLogger::tick(void) {
	wrefresh(_win);
}

std::string		logs::TermLogger::flush() {
	std::string	ret = this->_stream.str();
	this->_stream = std::stringstream();
	return (ret);
}
std::string		logs::TermLogger::print() {
	std::string	ret = this->flush();
	::mvwprintw(_win, this->_y, this->_x, ret.c_str());
	this->_y++;
	return (ret);
}
