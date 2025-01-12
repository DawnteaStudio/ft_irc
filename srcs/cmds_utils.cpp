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

void Server::makeJoinVector(Request &request, std::vector<std::string> &channels, std::vector<std::string> &keys)
{
	std::string channel = request.args[0];
	std::string key = "";

	if (request.args.size() > 1)
		key = request.args[1];
	while (channel.find(',') != std::string::npos) {
		channels.push_back(channel.substr(0, channel.find(',')));
		channel = channel.substr(channel.find(',') + 1);
	}
	channels.push_back(channel);
	if (keys.size() == 0)
		return;
	while (key.find(',') != std::string::npos) {
		keys.push_back(key.substr(0, key.find(',')));
		key = key.substr(key.find(',') + 1);
	}
	keys.push_back(key);
}

bool Server::isCharString(const char &c) const
{
	if (c == ' ' || c == static_cast<char>(0) || c == static_cast<char>(7) || c == static_cast<char>(10) || c == static_cast<char>(13))
		return false;
	return true;
}

ErrorCode Server::join(const std::string &channelName, const std::string &key, int fd, bool isInvite)
{
	if (this->channels.find(channelName) == this->channels.end()) {
		if (isInvite)
			return ERR_NOSUCHCHANNEL;
		if (channelName.length() < 2)
			return ERR_NOSUCHCHANNEL;
		if (this->clients[fd]->getChannels().size() >= 10)
			return ERR_TOOMANYCHANNELS;
		if (channelName[0] != '#' && channelName[0] != '&')
			return ERR_BADCHANNELNAME;
		if (channelName.size() > 200)
			return ERR_BADCHANNELNAME;
		for (size_t i = 1; i < channelName.size(); i++) {
			if (isCharString(channelName[i]) == false)
				return ERR_BADCHANNELNAME;
		}
		Channel *newChannel = new Channel(channelName);
		this->channels[channelName] = newChannel;
		this->channels[channelName]->addMember(this->clients[fd]);
		this->channels[channelName]->addOperator(this->clients[fd]);
		this->clients[fd]->addChannel(newChannel);
		return ERR_NONE;
	}
	if (this->channels[channelName]->isMember(fd))
		return ERR_ALREADYREGISTRED;
	if (this->channels[channelName]->getIsInviteOnly() && !isInvite) {
		if (!this->channels[channelName]->isOperator(fd))
			return ERR_INVITEONLYCHAN;
	}
	if (this->channels[channelName]->getIsKeyRequired() && !isInvite) {
		if (key != this->channels[channelName]->getKey())
			return ERR_BADCHANNELKEY;
	}
	if (this->channels[channelName]->getIsLimit() && this->channels[channelName]->getLimit() <= static_cast<int>(this->channels[channelName]->getMembers().size()))
		return ERR_CHANNELISFULL;
	this->channels[channelName]->addMember(this->clients[fd]);
	this->clients[fd]->addChannel(this->channels[channelName]);
	return ERR_NONE;
}