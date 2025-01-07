#include "../include/Server.hpp"

void Server::quit(int fd)
{
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	Client *client = this->clients[fd];
	if (client->getChannels().size() > 0)
	{
		for (std::vector<Channel *>::iterator it = client->getChannels().begin(); it != client->getChannels().end(); it++)
			(*it)->quit(client);
	}
	this->removeClient(fd);
	close(fd);
}

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
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end();)
	{
		if (it->first == fd)
		{
			delete it->second;
			this->clients.erase(it);
			break;
		}
		else
			it++;
	}
}