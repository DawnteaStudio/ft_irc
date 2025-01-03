#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <string>
# include "Client.hpp"
# include "Request.hpp"
# include "Error.hpp"

# define MAX_EVENT_COUNT 10

class Server {
	private:
		int socketFd;
		std::string port;
		std::string password;
		std::string name;
		std::map<int, Client *> clients;
		std::vector<std::string> clientNicknames;
		Server(const Server&);
		Server &operator=(const Server&);
		std::string createMessage(const int, const std::string&, const std::string&);
		std::string setPassword(Request&, int);
		std::string setUserNickname(Request&, int);
		// std::string setUser(Request&, int);
		// std::string	setOper(Request&, int);
		// std::string	quit(Request&, int);
		// std::string	joinChannel(Request&, int);
		// std::string	part(Request&, int);
		// std::string	setMode(Request&, int);
		// std::string	topic(Request&, int);
		// std::string	invite(Request&, int);
		// std::string	kick(Request&, int);
		// std::string	privmsg(Request&, int);
		// std::string	notice(Request&, int);
	public:
		Server();
		Server(const std::string&, const std::string&);
		~Server();
		void run();
};

#endif