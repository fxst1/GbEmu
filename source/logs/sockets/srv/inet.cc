#include "logs/SocketLogger.hh"

logs::ServerSocketLogger::ServerSocketLogger(std::string const &name, int port, int max_clients):
	logs::SocketLogger(name, port),
	_max_client(max_clients),
	_rfds(),
	_wfds()
{}
logs::ServerSocketLogger::~ServerSocketLogger(void)
{
	for (int &fd: _clients) {
		::shutdown(_sock, SHUT_RDWR);
		::close(fd);
	}
	if (_sock > 0) {
		::shutdown(_sock, SHUT_RDWR);
		::close(_sock);
	}
}

bool		logs::ServerSocketLogger::open(void) {
	_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cerr << "ServerSocketLogger: socket error" << std::endl;
		return (false);
	}

	int	enabled = 1;
	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0) {
		std::cerr << "Setsockopt(SO_REUSEADDR) failed" << std::endl;
	}

	_serv_inet_addr.sin_family = AF_INET;
	_serv_inet_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_inet_addr.sin_port = htons(_port);
	if (::bind(_sock, (struct sockaddr *)&_serv_inet_addr, sizeof(_serv_inet_addr)) < 0) {
		std::cerr << "ServerSocketLogger: bind error" << std::endl;
		return (false);
	}

	if (::listen(_sock, _max_client) < 0) {
		std::cerr << "ServerSocketLogger: listen error" << std::endl;
		return (false);
	}

	return (true);
}

int			logs::ServerSocketLogger::acceptClient(void) {
	struct sockaddr_in	cli_addr;
	socklen_t			clilen;

	int tmpsock = ::accept(_sock, (struct sockaddr *) &cli_addr, &clilen);
	if (tmpsock > 0) {
		std::cout << "* Client Accepted" << std::endl;
		_clients.push_front(tmpsock);
	} else
		std::cerr << "ServerSocketLogger: accept error" << std::endl;
	return (tmpsock);
}

void		logs::ServerSocketLogger::kickClient(int cli) {
	for (auto it = _clients.begin(); it != _clients.end(); it++) {
		if (*it == cli) {
			std::cout << "* Client " << *it << " has leaved" << std::endl;
			::shutdown(_sock, SHUT_RDWR);
			::close(*it);
			_clients.erase(it);
			break ;
		}
	}
}

void		logs::ServerSocketLogger::tick(void) {
	int		nfds = _sock;

	FD_ZERO(&_rfds);
	FD_SET(_sock, &_rfds);
	for (auto &it : _clients) {
		FD_SET(it, &_rfds);
		if (nfds < it) nfds = it;
	}

	if (::select(nfds + 1, &_rfds, NULL, NULL, NULL) < 0) {
		return ;
	}

	if (FD_ISSET(_sock, &_rfds)) {
		this->onClientConnect();
	}

	for (auto &it: _clients) {
		if (FD_ISSET(it, &_rfds)) {
			ssize_t	n = this->receive(it, 0);
			if (n <= 0)
				this->kickClient(it);
			else
				this->onReceive(it);
		}
	}
}
