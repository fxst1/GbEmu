#include "logs/SocketLogger.hh"

logs::ClientSocketLogger::ClientSocketLogger(std::string const &name, std::string const &hostname, int port):
	logs::SocketLogger(name, port),
	_hostname(hostname)
{}
logs::ClientSocketLogger::~ClientSocketLogger(void) {
	::shutdown(_sock, SHUT_RDWR);
	::close(_sock);
}
bool	logs::ClientSocketLogger::open(void) {
	_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0) {
		std::cerr << "ClientSocketLogger: socket error" << std::endl;
		return (false);
	}

	_server = ::gethostbyname(_hostname.c_str());
	if (_server == nullptr) {
		std::cerr << "ClientSocketLogger: gethostbyname error" << std::endl;
		return (false);
	}

	::bzero((char *) &_serv_inet_addr, sizeof(_serv_inet_addr));
	_serv_inet_addr.sin_family = AF_INET;
	::bcopy((char *)_server->h_addr,
		 (char *)&_serv_inet_addr.sin_addr.s_addr,
		 _server->h_length);
	_serv_inet_addr.sin_port = htons(_port);
	if (::connect(_sock, (struct sockaddr*)&_serv_inet_addr, sizeof(_serv_inet_addr)) < 0) {
		std::cerr << "ClientSocketLogger: connect error" << std::endl;
		return (false);
	}
	return (true);
}
