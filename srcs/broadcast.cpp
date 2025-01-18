#include "../include/Server.hpp"

void Server::broadcastChannel(const std::string &channelName, const std::string &response)
{
	std::map<int, Client *> members = this->channels[channelName]->getMembers();
	std::map<int, Client *>::iterator iter = members.begin();
	
	for (; iter != members.end(); iter++)
		send(iter->first, response.c_str(), response.length(), 0);
}

// std::string Server::makeBroadMsg(const std::string &response, int fd)
// {
// 	std::string res = ":" + this->clients[fd]->getNickname() + "!" + this->clients[fd]->getUserName() + "@" + this->clients[fd]->getIpAddr() + " " + response;
// 	res += "\r\n";
// 	return res;
// }

