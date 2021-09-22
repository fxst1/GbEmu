#pragma once

# include <list>
# include <cerrno>
# include <cstring>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/un.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <netdb.h>

# include "logs/Logs.hh"
# include "timing/Timing.hh"

namespace	logs {

	class	SocketLogger: public logs::ILogger, public timing::Tick {
		public:
			SocketLogger(std::string const & name, int port);
			~SocketLogger(void);

			virtual void			tick(void) = 0;

			ssize_t					receive(int flags);
			ssize_t					receive(int cli, int flags);
			ssize_t					send(int flags);
			ssize_t					send(int cli, int flags);

			virtual bool			open(void) = 0;
			virtual bool			openFromFile(std::string const &filepath, bool open_has_server = false);


			virtual std::ostream&	output(void);
			virtual std::istream&	input(void);

		protected:
			int						_port;
			std::stringstream		_input;
			std::stringstream		_output;
			int						_sock;
	};

	class	ServerSocketLogger: public logs::SocketLogger {
		public:
			ServerSocketLogger(std::string const &name, int port, int max_clients);
			virtual ~ServerSocketLogger(void);

			int					acceptClient(void);
			void				kickClient(int cli);

			virtual void		tick(void);
			void				broadcast(void);

			virtual bool		open(void);

			virtual void		onClientConnect(void) = 0;
			virtual void		onClientDisconnect(int sock) = 0;
			virtual void		onReceive(int sock) = 0;

		protected:
			int					_max_client;

			union {
				struct sockaddr_in	_serv_inet_addr;
				struct sockaddr_un	_serv_unix_addr;
			};

			std::list<int>		_clients;
			fd_set				_rfds;
			fd_set				_wfds;
	};

	class	ClientSocketLogger: public logs::SocketLogger {
		public:
			ClientSocketLogger(std::string const &name, std::string const &hostname, int port);
			virtual ~ClientSocketLogger(void);

			virtual void			tick(void) = 0;
			virtual bool			open(void);

		protected:
			std::string				_hostname;
			union {
				struct sockaddr_in	_serv_inet_addr;
				struct sockaddr_un	_serv_unix_addr;
			};
			struct hostent*			_server;

	};

	class	LocalServerSocketLogger: public  logs::ServerSocketLogger {
		public:
			LocalServerSocketLogger(std::string const &name, std::string const &sock_path, int max_clients);
			virtual ~LocalServerSocketLogger(void);
			virtual bool			open(void);
		protected:
			std::string				_sockpath;
	};

	class	LocalClientSocketLogger: public  logs::ClientSocketLogger {
		public:
			LocalClientSocketLogger(std::string const &name, std::string const &hostname);
			virtual bool			open(void);
	};
};
