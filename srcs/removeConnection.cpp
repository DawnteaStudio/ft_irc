#include "../include/Server.hpp"

void Server::removeClient(int fd)
{
	for (std::vector<pollfd>::iterator it = this->pfd.begin(); it != this->pfd.end();)
	{
		if (it->fd == fd)
		{
			this->pfd.erase(it);
			break;
		}
		else
			it++;
	}
	delete this->clients[fd];
	this->clients.erase(fd);
}