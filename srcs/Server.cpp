#include "../include/Server.hpp"

Server::Server() : socketFd(0), port(), password(), name("irc_server"), clients(), clientNicknames(), clientCnt(MAX_EVENT_COUNT) {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password), name("irc_server"), clients(), clientNicknames(), clientCnt(MAX_EVENT_COUNT) {
	setSocket();
	this->pfd->at(0).fd = this->socketFd;
	this->pfd->at(0).events = POLLIN;
	this->clientCnt++;
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

void Server::run() {
	while (true) {
		int event_cnt = poll(this->pfd->data(), this->clientCnt, 0);

		if (event_cnt == -1)
			throw std::runtime_error("Poll() error!");
		if (event_cnt == 0)
			continue;
		for (int i = 0; i < this->clientCnt; i++)
		{
			if (this->pfd->at(i).revents & POLLIN)
			{
				// if (this->pfd->at(i).fd == this->socketFd)
					// _newClient();			// If listener is ready to read, handle new connection
				// else
					// _ClientRequest(i);		// If not the listener, we're just a regular client
			}
		}
	}
}

