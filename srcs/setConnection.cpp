#include "../include/Server.hpp"

void Server::setSocket() {

	int tmp_port = atoi((this->port).c_str());
	if (tmp_port <= 0 || tmp_port > 65535)
		throw std::runtime_error("Wrong port Number!");

	this->socketFd = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		throw std::runtime_error("socket() error!");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons((in_port_t)tmp_port);


	if (fcntl(this->socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl() error!");
	if (bind(this->socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		throw std::runtime_error("bind() error!");
	if (listen(this->socketFd, SOMAXCONN) == -1)
		throw std::runtime_error("listen() error!");
	this->serverStruct.fd = this->socketFd;
	this->serverStruct.events = POLLIN;
	this->serverStruct.revents = 0;
	this->pfd.push_back(this->serverStruct);
}

void Server::addClient() {
	struct sockaddr_in clientAddr;
	struct pollfd new_poll;
	socklen_t addrLen = sizeof(clientAddr);

	int clientFd = accept(this->socketFd, (struct sockaddr *)&clientAddr, &addrLen);
	if (clientFd == -1)
		throw std::runtime_error("Accept() error!");
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl() error!");

	char clientIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
	std::cout << "New client connected: " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

	new_poll.fd = clientFd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	this->pfd.push_back(new_poll);
	this->clients.insert(std::pair<int, Client *>(clientFd, new Client(clientFd)));
}

void Server::connectClient(int fd) {
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesReceived = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytesReceived <= 0) {
		
	}
}