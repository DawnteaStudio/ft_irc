#include "../include/Server.hpp"

Server::Server() : socketFd(0), name("irc_server") {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password), name("irc_server") {
	setSocket();
}

Server::~Server()
{
	std::map<int, Client *>::iterator it = this->clients.begin();
	while (it != this->clients.end()) {
		delete it->second;
		it++;
	}
	std::map<std::string, Channel *>::iterator it2 = this->channels.begin();
	while (it2 != this->channels.end()) {
		delete it2->second;
		it2++;
	}
	close(this->socketFd);
}

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
	while (sig::stopServer == false) {
		int event_cnt = poll(&this->pfd[0], this->pfd.size(), 0);

		if (event_cnt == -1)
			throw std::runtime_error("poll() error!");
		if (event_cnt == 0)
			continue;
		for (size_t i = 0; i < this->pfd.size(); i++) {
			if (this->pfd[i].revents & POLLIN) {
				if (this->pfd[i].fd == this->socketFd)
					addClient();
				else
					connectClient(this->pfd[i].fd);
			}
		}
	}
}

