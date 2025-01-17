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
# include <fstream>
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
# define DOWNLOADED_FILE_PATH "/Users/sewopark/Desktop/downloads/" // envp 고려(user name)

class Client;
class Channel;

class Server {
	private:
		int socketFd;
		std::string port;
		std::string password;
		std::string name;
		std::map<int, Client *> clients;
		std::map<std::string, Channel *> channels;
		std::vector<std::string> clientNicknames;
		struct pollfd serverStruct;
		std::vector<pollfd> pfd;
		Server(const Server&);
		Server &operator=(const Server&);
		void makeVector(std::string, std::vector<std::string>&);
		void setSocket();
		void addClient();
		void connectClient(int);
		void quit(int);
		void removeClient(int);
		void execCmd(Request&, int);
		std::string setPassword(Request&, int);
		std::string setUserNickname(Request&, int);
		std::string setUser(Request&, int);
		std::string getFile(Request&, int); //GETFILE <channel> <filename>
		std::string sendFile(Request&, int); //SENDFILE <channel> <filename>
		// std::string	quit(Request&, int);
		std::string joinChannel(Request&, int);
		ErrorCode join(const std::string&, const std::string&, int);
		std::string	partChannel(Request&, int);
		ErrorCode part(const std::string&, int);
		std::string kickUser(Request&, int);
		ErrorCode kick(const std::string&, const std::string&, int);
		// std::string	setMode(Request&, int);
		// std::string	topic(Request&, int);
		// std::string	invite(Request&, int);
		// std::string	privmsg(Request&, int);
		// std::string	notice(Request&, int);
		bool isValidUserNickname(const std::string&);
		bool isUsedUserNickname(const std::string&);
		void deleteUserNickname(const std::string&);
		void addNewUserNickname(const std::string&);
		bool isSameNickname(const std::string&, const std::string&);
		bool isCharString(const char&) const;
		std::string convertChar(const std::string&);
		void broadcastChannel(const std::string&, const std::string&);
		Client *getClientByNickname(const std::string&);
	public:
		Server();
		Server(const std::string&, const std::string&);
		~Server();
		void run();
};

#endif