#include "../include/Server.hpp"

std::string Server::convertChar(const std::string &str)
{
	std::string tmp = str;
	int idx = 0, size = static_cast<int>(str.size());

	while (idx < size) {
		if (tmp[idx] == '{') tmp[idx] = '[';
		else if (tmp[idx] == '}') tmp[idx] = ']';
		else if (tmp[idx] == '\\') tmp[idx] = '|';
		idx++;
	}
	return tmp;
}

bool Server::isValidUserNickname(const std::string &nickname)
{
	int idx = 0, size = static_cast<int>(nickname.size());
	if (size > 9 || nickname == "") return false;

	std::string allowedChars = "-[]{}|'^\\";
	while (idx < size) {
		if (!std::isalnum(nickname[idx]) && allowedChars.find(nickname[idx]) == std::string::npos)
			return false;
		idx++;
	}
	return true;
}

bool Server::isUsedUserNickname(const std::string &nickname)
{
	std::string tmp = convertChar(nickname);
	std::vector<std::string>::iterator iter = std::find(this->clientNicknames.begin(), this->clientNicknames.end(), tmp);
	if (iter != this->clientNicknames.end())
		return true;
	return false;
}

void Server::deleteUserNickname(const std::string &nickname)
{
	std::string tmp = convertChar(nickname);
	std::vector<std::string>::iterator iter = std::find(this->clientNicknames.begin(), this->clientNicknames.end(), tmp);
	if (iter != this->clientNicknames.end())
		this->clientNicknames.erase(iter);
}

void Server::addNewUserNickname(const std::string &newNickname)
{
	std::string tmp = convertChar(newNickname);
	this->clientNicknames.push_back(tmp);
}

bool Server::isSameNickname(const std::string &newNickname, const std::string &nickname)
{
	return (nickname == newNickname);
}

bool Server::isCharString(const char &c) const
{
	if (c == ' ' || c == static_cast<char>(0) || c == static_cast<char>(7) || c == static_cast<char>(10) || c == static_cast<char>(13))
		return false;
	return true;
}

Client * Server::getClientByNickname(const std::string &nickname)
{
	std::string tmp = convertChar(nickname);
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
		if (it->second->getNickname() == tmp)
			return it->second;
	}
	return NULL;
}

ErrorCode Server::join(const std::string &channelName, const std::string &key, int fd)
{
	if (this->channels.find(channelName) == this->channels.end()) {
		if (channelName.length() < 2)
			return ERR_NOSUCHCHANNEL;
		if (this->clients[fd]->getChannels().size() >= 10)
			return ERR_TOOMANYCHANNELS;
		if (channelName[0] != '#' && channelName[0] != '&')
			return ERR_NOSUCHCHANNEL;
		if (channelName.size() > 200)
			return ERR_NOSUCHCHANNEL;
		for (size_t i = 1; i < channelName.size(); i++) {
			if (isCharString(channelName[i]) == false)
				return ERR_NOSUCHCHANNEL;
		}
		Channel *newChannel = new Channel(channelName);
		this->channels[channelName] = newChannel;
		this->channels[channelName]->addMember(this->clients[fd]);
		this->channels[channelName]->addOperator(this->clients[fd]);
		this->clients[fd]->addChannel(newChannel);

		broadcastChannel(channelName, Response::customMessageForJoin(this->clients[fd]->getPrefix(), channelName));
		channelInfo(fd, channelName);
		return ERR_NONE;
	}
	if (this->channels[channelName]->getIsInviteOnly()) {
		if (!this->channels[channelName]->isOperator(fd))
			return ERR_INVITEONLYCHAN;
	}
	if (this->channels[channelName]->getIsKeyRequired()) {
		if (key != this->channels[channelName]->getKey())
			return ERR_BADCHANNELKEY;
	}
	if (this->channels[channelName]->getIsLimit() && this->channels[channelName]->getLimit() <= static_cast<int>(this->channels[channelName]->getMembers().size()))
		return ERR_CHANNELISFULL;

	this->channels[channelName]->addMember(this->clients[fd]);
	this->clients[fd]->addChannel(this->channels[channelName]);
	
	broadcastChannel(channelName, Response::customMessageForJoin(this->clients[fd]->getPrefix(), channelName));
	channelInfo(fd, channelName);
	return ERR_NONE;
}

ErrorCode Server::part(const std::string &channelName, int fd)
{
	if (this->channels.find(channelName) == this->channels.end())
		return ERR_NOSUCHCHANNEL;
	if (this->channels[channelName]->isOperator(fd))
		this->channels[channelName]->removeOperator(fd);
	if (!this->channels[channelName]->isMember(fd))
		return ERR_NOTONCHANNEL;
	// broadcast
	this->channels[channelName]->removeMember(fd);
	this->clients[fd]->removeChannel(this->channels[channelName]);
	if (this->channels[channelName]->getMembers().size() == 0) {
		delete this->channels[channelName];
		this->channels.erase(channelName);
	}
	return ERR_NONE;
}

ErrorCode Server::kick(const std::string &channelName, const std::string &nickname, int fd)
{
	if (this->channels.find(channelName) == this->channels.end())
		return ERR_NOSUCHCHANNEL;
	if (nickname.empty())
		return ERR_NEEDMOREPARAMS;
	if (!this->channels[channelName]->isMember(fd))
		return ERR_NOTONCHANNEL;
	if (!this->channels[channelName]->isOperator(fd))
		return ERR_CHANOPRIVSNEEDED;
		
	Client *kickClient = getClientByNickname(nickname);
	if (kickClient == NULL || !this->channels[channelName]->isMember(kickClient->getClientFd()))
		return ERR_NOTONCHANNEL;

	int kickFd = kickClient->getClientFd();
	if (this->channels[channelName]->isOperator(kickFd))
		this->channels[channelName]->removeOperator(kickFd);
	this->channels[channelName]->removeMember(kickFd);
	this->clients[kickFd]->removeChannel(this->channels[channelName]);
	// broadcast
	return ERR_NONE;
}

void Server::makeVector(std::string str, std::vector<std::string> &vec)
{
	while (str.find(',') != std::string::npos) {
		vec.push_back(str.substr(0, str.find(',')));
		str = str.substr(str.find(',') + 1);
	}
	if (!str.empty())
		vec.push_back(str);
}

void Server::channelInfo(const int &fd, const std::string &channelName)
{
	Channel *channel = this->channels[channelName];
	std::string response;
	std::string prefix = this->clients[fd]->getPrefix();
	std::string nickname = this->clients[fd]->getNickname();

	// topic 
	std::string topic = channel->getTopic();
	if (!topic.empty()) {
		response = Response::success(RPL_TOPIC, channelName, prefix, nickname, topic);
		send(fd, response.c_str(), response.length(), 0);
	}

	// name list
	std::string nameList;
	std::map<int, Client *> members = channel->getMembers();
	std::map<int, Client *>::iterator iter = members.begin();
	for (; iter != members.end(); iter++) {
		if (channel->isOperator(iter->first))
			nameList += "@";
		nameList += iter->second->getNickname() + " ";
	}
	nameList.erase(nameList.size() - 1, 1);
	response = Response::success(RPL_NAMREPLY, channelName, prefix, nickname, nameList);
	send(fd, response.c_str(), response.length(), 0);

	response = Response::success(RPL_ENDOFNAMES, channelName, prefix, nickname, "");
	send(fd, response.c_str(), response.length(), 0);
}