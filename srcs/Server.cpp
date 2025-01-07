#include "../include/Server.hpp"

Server::Server() : socketFd(0), name("irc_server") {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password), name("irc_server") {
	setSocket();
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

void Server::run() {
	while (true) {
		int event_cnt = poll(&this->pfd[0], this->pfd.size(), 0);

		if (event_cnt == -1)
			throw std::runtime_error("poll() error!");
		if (event_cnt == 0)
			continue;
		for (size_t i = 0; i < this->pfd.size(); i++)
		{
			if (this->pfd[i].revents & POLLIN)
			{
				if (this->pfd[i].fd == this->socketFd)
					addClient();
				else
					connectClient(this->pfd[i].fd);
			}
		}
	}
}

