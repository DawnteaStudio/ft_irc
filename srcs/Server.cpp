#include "../include/Server.hpp"

Server::Server() : socket_fd(0), port(), password(), name("irc_server"), clients(), client_nicknames() {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password), name("irc_server"), clients(), client_nicknames()
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

void Server::run()
{
	while (true) {
		// socket communication
		// add new client ex) new Client(this->name, client_fd);
		// receive new messages
	}
}