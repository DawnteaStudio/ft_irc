#include "../include/Server.hpp"

void Server::setSocket() {

	this->socketFd = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		throw std::runtime_error("socket() error!");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi((this->port).c_str()));

	fcntl(this->socketFd, F_SETFL, O_NONBLOCK);
	if (bind(this->socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		throw std::runtime_error("bind() error!");
	if (listen(this->socketFd, this->clientCnt) == -1)
		throw std::runtime_error("listen() error!");
	this->pfd = new std::vector<pollfd>(this->clientCnt);
}

void Server::newClient() {
	
}