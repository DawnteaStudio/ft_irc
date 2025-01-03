#include "../include/Server.hpp"

Server::Server() : socketFd(0), port(), password(), name("irc_server"), clients(), clientNicknames() {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password), name("irc_server"), clients(), clientNicknames()
{
	// setSocketFd()
}

Server::~Server() {}

Server::Server(const Server &other)
{
	(void)other;
}

Server &Server::operator=(const Server &other)
{
	(void)other;
	return *this;
}

std::string Server::createMessage(const int num, const std::string &client_nickname, const std::string &message)
{
	
}

void Server::run()
{
	while (true) {
		// socket communication
		// add new client ex) new Client(this->name, client_fd);
		// receive new messages
	}
}