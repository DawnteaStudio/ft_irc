#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <string>
# include <stdexcept>
# include <sstream>
# include <poll.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include "Client.hpp"
# include "Request.hpp"
# include "Channel.hpp"
# include "Response.hpp"

# define BUFFER_SIZE 1024
# define RED "\033[1;31m"
# define WHITE "\033[0;37m"

class Client;
class Channel;

class Server {
	private:
		int socketFd;
		std::string port;
		std::string password;
		std::string name;
		std::map<int, Client *> clients;
		std::vector<std::string> clientNicknames;
		struct pollfd serverStruct;
		std::vector<pollfd> pfd;
		Server(const Server&);
		Server &operator=(const Server&);
		void setSocket();
		void addClient();
		void connectClient(int);
		void quit(int);
		void removeClient(int);
		void execCmd(Request&, int);
		std::string createMessage(const int, const std::string&, const std::string&);
		std::string setPassword(Request&, int);
		std::string setUserNickname(Request&, int);
		std::string setUser(Request&, int);
		std::string setOper(Request&, int);
		// std::string	quit(Request&, int);
		// std::string	joinChannel(Request&, int);
		// std::string	part(Request&, int);
		// std::string	setMode(Request&, int);
		// std::string	topic(Request&, int);
		// std::string	invite(Request&, int);
		// std::string	kick(Request&, int);
		// std::string	privmsg(Request&, int);
		// std::string	notice(Request&, int);
		bool isValidUserNickname(const std::string&);
		bool isUsedUserNickname(const std::string&);
		void deleteUserNickname(const std::string&);
		void addNewUserNickname(const std::string&);
		std::string convertChar(const std::string&);
	public:
		Server();
		Server(const std::string&, const std::string&);
		~Server();
		void run();
};

#endif