#include "logs/SocketLogger.hh"

logs::LocalServerSocketLogger::LocalServerSocketLogger(std::string const &name, std::string const &sock_path, int max_clients) :
	logs::ServerSocketLogger(name, -1, max_clients),
	_sockpath(sock_path)
{}
logs::LocalServerSocketLogger::~LocalServerSocketLogger(void) {

	if (_sock > 0) {
		std::cout << "* Cleaning socket file: '" << _sockpath << "'" << std::endl;
		::shutdown(_sock, SHUT_RDWR);
		::close(_sock);
		::unlink(_sockpath.c_str());
		_sock = -1;
	}
}

bool		logs::LocalServerSocketLogger::open(void) {

	int		tmp = ::access(_sockpath.c_str(), F_OK);
	if (tmp == 0) {
		std::cout << "* Cleaning socket file: '" << _sockpath << "'" << std::endl;
		::shutdown(_sock, SHUT_RDWR);
		::close(_sock);
		::unlink(_sockpath.c_str());
		_sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if (_sock < 0) {
			std::cerr << "LocalServerSocketLogger: socket error: " << std::endl;
			return (false);
		}
	} else {
		_sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if (_sock < 0) {
			std::cerr << "LocalServerSocketLogger: socket error: " << std::endl;
			return (false);
		}
	}


	int v = 1;
	if (::setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int)) < 0) {
		std::cerr << "LocalServerSocketLogger: socket option error (SO_REUSEADDR)" << std::endl;
		return (false);
	}

	_serv_unix_addr.sun_family = AF_UNIX;
	::strncpy(_serv_unix_addr.sun_path, _sockpath.c_str(), sizeof(_serv_unix_addr.sun_path));
	if (::bind(_sock, (struct sockaddr *)&_serv_unix_addr, sizeof(_serv_unix_addr)) < 0) {
		std::cerr << "LocalServerSocketLogger: bind error: " << strerror(errno) << ": " << errno << std::endl;
		return (false);
	}

	if (::listen(_sock, _max_client) < 0) {
		std::cerr << "LocalServerSocketLogger: listen error" << strerror(errno) << ": " << errno << std::endl;
		return (false);
	}

	return (true);
}
