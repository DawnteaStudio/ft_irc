#include "../include/Server.hpp"

void Server::removeClient(int fd, bool isLeave, const std::string &reason)
{
	int size = this->clients[fd]->getChannels().size();
	for (int i = 0; i < size; i++) {
		std::string channelName = this->clients[fd]->getChannels()[i]->getName();
		Channel *channel = this->channels[channelName];
		channel->removeMember(fd);
		// braodcastChannel(channelName, makeBroadMsg("QUIT " + channelName, fd));
		// if (isLeave) :Quit: leaving
		// else :Quit sigint
		if (channel->isOperator(fd))
			channel->removeOperator(fd);
		if (channel->getMembers().size() == 0)
			removeChannelData(channelName);
	}
	this->deleteUserNickname(this->clients[fd]->getNickname());
	delete this->clients[fd];
	this->clients.erase(fd);
}

void Server::removeChannelInvitedClient(Channel *channel)
{
	std::string channelName = channel->getName();
	std::vector<int> invitedClients = channel->getInvitedClients();
	for (std::vector<int>::iterator it = invitedClients.begin(); it != invitedClients.end(); it++)
		this->clients[*it]->removeInvitedChannel(channelName);
}

void Server::removeChannelData(const std::string &channelName)
{
	removeChannelInvitedClient(this->channels[channelName]);
	delete this->channels[channelName];
	this->channels.erase(channelName);
}