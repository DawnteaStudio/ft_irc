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

std::string Server::createMessage(const int num, const std::string &clientNickname, const std::string &message)
{
	std::string tmp = clientNickname;
	if (tmp == "")
		tmp = "*";
	return ":" + this->name + " " + std::to_string(num) + " " + tmp + " " + message;
}

void Server::run()
{
	while (true) {
		// socket communication
		// add new client ex) new Client(this->name, client_fd);
		// receive new messages
	}
}