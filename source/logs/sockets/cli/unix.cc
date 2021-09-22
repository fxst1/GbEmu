#include "logs/SocketLogger.hh"

logs::LocalClientSocketLogger::LocalClientSocketLogger(std::string const &name, std::string const &sock_path) :
	logs::ClientSocketLogger(name, sock_path, -1)
{
	if (this->open() == false) {
		std::cout << "ERROR : " << sock_path << std::endl;
	}
}

bool		logs::LocalClientSocketLogger::open(void) {
	_sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cerr << "LocalClientSocketLogger: socket error" << std::endl;
		return (false);
	}

	_serv_unix_addr.sun_family = AF_UNIX;
	::strncpy(_serv_unix_addr.sun_path, _hostname.c_str(), sizeof(_serv_unix_addr.sun_path));
	if (::connect(_sock, (struct sockaddr *)&_serv_unix_addr, sizeof(_serv_unix_addr)) < 0) {
		std::cerr << "LocalClientSocketLogger: connect error" << strerror(errno) << " " << errno << std::endl;
		return (false);
	}
	return (true);
}
