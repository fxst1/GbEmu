#include "logs/SocketLogger.hh"

logs::SocketLogger::SocketLogger(std::string const &name, int port):
	logs::ILogger(name), _port(port), _input(), _output()
{}
logs::SocketLogger::~SocketLogger(void){}

bool			logs::SocketLogger::openFromFile(std::string const &f, bool open_has_server) {
	_sock = ::open(f.c_str(), (open_has_server) ? O_RDWR | O_CREAT : O_RDWR);
	if (_sock < 0) {
		std::cerr << "SocketLogger: Could not open '" << f << "'" << std::endl;
		return (false);
	} else {
		std::cout << "SocketLogger: Socket path '" << f << "'" << std::endl;
	}
	return (true);
}

ssize_t			logs::SocketLogger::send(int sock, int flags) {

	if (flags == 0) flags = MSG_WAITALL;

//	std::cout << "BEGIN Send" << std::endl;
//	std::cout << this->_input.str() << std::endl;

//	ssize_t	n = ::send(sock, this->_input.str().c_str(), this->_input.str().size(), flags);
	ssize_t	n = ::write(sock, this->_input.str().c_str(), this->_input.str().size() + 1);
	(void) flags;

	this->_input = std::stringstream();
	if (n < 0) std::cout << "SocketLogger: send error: " << std::strerror(errno) << std::endl;
//	std::cout << "END Send" << std::endl;
	(void)n;
	return (n);
}
ssize_t			logs::SocketLogger::receive(int sock, int flags) {
	ssize_t		n = 0;
	char*		buf = new char[256];

	if (flags == 0) flags = MSG_WAITALL;
	(void)flags;

	::bzero(buf, 256);
	this->_output = std::stringstream();
//	std::cout << "BEGIN Received" << std::endl;
	while ((n = ::recv(sock, buf, 255, 0)) > 0) {
//		std::cout << "- " << std::dec << (int)n << ": " << buf << std::endl;
		this->_output << buf;
		if (n < 255 || ::strchr(buf, '\n')) break;
		::bzero(buf, 255);
	}
	(void)flags;
	if (n < 0) std::cout << "SocketLogger: receive error: " << std::strerror(errno) << " ("<<errno<<")" << std::endl;
	delete[] buf;

//	std::cout << this->_output.str() << std::endl;
//	std::cout << "END Received" << std::endl;
	return (n);
}
ssize_t			logs::SocketLogger::receive(int flags) {
	return this->receive(_sock, flags);
}
ssize_t			logs::SocketLogger::send(int flags) {
	return this->send(_sock, flags);
}
std::ostream&	logs::SocketLogger::output(void) {
	return (_output);
}
std::istream&	logs::SocketLogger::input(void) {
	return (_input);
}
