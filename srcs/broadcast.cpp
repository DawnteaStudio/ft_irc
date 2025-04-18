#include "../include/Server.hpp"

void Server::broadcastChannel(const std::string &channelName, const std::string &response)
{
	std::map<int, Client *> members = this->channels[channelName]->getMembers();
	std::map<int, Client *>::iterator iter = members.begin();
	
	for (; iter != members.end(); iter++)
		send(iter->first, response.c_str(), response.length(), 0);
}

void Server::sendError(ErrorCode err, const std::string &channelName, int fd)
{
	std::string res = Response::failure(err, channelName, this->name, this->clients[fd]->getNickname());
	send(fd, res.c_str(), res.length(), 0);
}