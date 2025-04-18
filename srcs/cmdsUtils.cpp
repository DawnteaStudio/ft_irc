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

void Server::makeUpper(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

bool Server::isClientInServer(const std::string &nickname)
{
	int size = this->clientNicknames.size();
	for (int i = 0; i < size; i++) {
		if (this->clientNicknames[i] == nickname)
			return true;
	}
	return false;
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

bool Server::isValidChannelName(const std::string &name)
{
	if (name.length() < 1 || name.length() > 200)
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.length(); i++) {
		if (!isCharString(name[i]))
			return false;
	}
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
		if (this->clients[fd]->getChannels().size() >= 10)
			return ERR_TOOMANYCHANNELS;
		if (!isValidChannelName(channelName))
			return ERR_BADCHANNAME;
		if (channelName.size() > 200)
			return ERR_TOOMANYCHANNELS;
		Channel *newChannel = new Channel(channelName);
		this->channels[channelName] = newChannel;
		this->channels[channelName]->addMember(this->clients[fd]);
		this->channels[channelName]->addOperator(this->clients[fd]);
		this->clients[fd]->addChannel(newChannel);

		broadcastChannel(channelName, Response::customMessageForJoin(this->clients[fd]->getPrefix(), channelName));
		channelInfo(fd, channelName);
		return ERR_NONE;
	}
	if (this->channels[channelName]->isMember(fd))
		return ERR_NONE;
	if (this->channels[channelName]->getIsInviteOnly()) {
		if (!this->channels[channelName]->isInvitedClient(fd))
			return ERR_INVITEONLYCHAN;
	}
	if (this->channels[channelName]->getIsKeyRequired()) {
		if (key != this->channels[channelName]->getKey())
			return ERR_BADCHANNELKEY;
	}
	if (this->channels[channelName]->getIsLimit() && this->channels[channelName]->getLimit() <= static_cast<int>(this->channels[channelName]->getMembers().size()))
		return ERR_CHANNELISFULL;
	if (this->clients[fd]->getChannels().size() >= 10)
		return ERR_TOOMANYCHANNELS;
	if (this->clients[fd]->isInvitedChannel(channelName)) {
		this->clients[fd]->removeInvitedChannel(channelName);
		this->channels[channelName]->removeInvitedClient(fd);
	}
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

	broadcastChannel(channelName, Response::customMessageForPart(this->clients[fd]->getPrefix(), channelName));
	this->channels[channelName]->removeMember(fd);
	this->clients[fd]->removeChannel(this->channels[channelName]);
	if (this->channels[channelName]->getMembers().size() == 0)
		removeChannelData(channelName);
	return ERR_NONE;
}

ErrorCode Server::kick(const std::string &channelName, const std::string &nickname, const std::string &reason, int fd)
{
	if (this->channels.find(channelName) == this->channels.end())
		return ERR_NOSUCHCHANNEL;

	Channel *channel = this->channels[channelName];
	if (nickname.empty())
		return ERR_NEEDMOREPARAMS;
	if (!channel->isMember(fd))
		return ERR_NOTONCHANNEL;
	if (!channel->isOperator(fd))
		return ERR_CHANOPRIVSNEEDED;
	if (this->isClientInServer(nickname) == false)
		return ERR_NOSUCHNICK;

	Client *kickClient = getClientByNickname(nickname);
	int kickFd = kickClient->getClientFd();
	if (channel->isOperator(kickFd))
		channel->removeOperator(kickFd);

	broadcastChannel(channelName, Response::customMessageForKick(this->clients[fd]->getPrefix(), channelName, nickname, reason));
	channel->removeMember(kickFd);
	this->clients[kickFd]->removeChannel(channel);
	return ERR_NONE;
}

std::string Server::modeInfo(Channel *channel, int fd)
{
	std::string modes = "+";
	std::string params;
	std::vector<char> modeVector = channel->getModeVector();
	std::vector<std::pair<char, std::string> > modeParams = channel->getModeParams();
	int modeSize = modeVector.size();
	int paramSize = modeParams.size();
	for (int i = 0; i < modeSize; i++)
		modes += modeVector[i];
	for (int i = 0; i < paramSize; i++) {
		if (i != paramSize - 1)
			params += modeParams[i].second + " ";
		else
			params += ":" + modeParams[i].second;
	}
	std::string res;
	if (paramSize == 0)
		res = ":" + modes;
	else
		res = modes + " " + params;
	return Response::success(RPL_CHANNELMODEIS, channel->getName(), this->clients[fd]->getPrefix(), this->clients[fd]->getNickname(), res);
}

void Server::classifyMode(Request &request, std::string &sendMsg, int fd)
{
	std::string ChannelName = request.args[0];
	std::vector<std::pair<char, std::string> > modes;
	std::vector<std::string> params(request.args.begin() + 2, request.args.end());
	makeModeVector(request.args[1], modes);

	size_t size = modes.size();
	std::string empty;
	std::vector<std::string> sendingParams;
	char sign = '+';
	ErrorCode err;
	for (size_t i = 0; i < size; i++) {
		if (!isRightModeFlag(modes[i].second)) {
			sendError(ERR_UNKNOWNMODE, modes[i].second, fd);
			continue;
		}
		if (isNeedParamFlag(modes[i].second, modes[i].first == '+')) {
			if (params.size() == 0) {
				sendError(ERR_NEEDMOREPARAMS, modes[i].second, fd);
				continue;
			}
			err = mode(ChannelName, modes[i], params[0]);
			if (err != ERR_NONE) {
				if (err != ERR_DONOTHING)
					sendError(err, modes[i].second, fd);
			}
			else
				sendingParams.push_back(params[0]);
			params.erase(params.begin());
		} else {
			err = mode(ChannelName, modes[i], empty);
			if (err != ERR_NONE)
				sendError(err, modes[i].second, fd);
		}

		if (err == ERR_NONE) {
			if (sign != modes[i].first) {
				sign = modes[i].first;
				sendMsg += sign;
			}
			else if (sendMsg == "")
				sendMsg += "+";
			sendMsg += modes[i].second;
		}
	}

	if (sendMsg == "")
		return;
	size_t sendingSize = sendingParams.size();
	if (sendingSize != 0)
		sendMsg += " ";
	else
		sendMsg = ":" + sendMsg;
	for (size_t i = 0; i < sendingSize; i++) {
		if (i != sendingSize - 1)
			sendMsg += sendingParams[i] + " ";
		else
			sendMsg += ":" + sendingParams[i];
	}
}

bool Server::isNeedParamFlag(const std::string &modeFlag, bool isAdd)
{
	if (modeFlag == "k" || modeFlag == "l" || modeFlag == "o") {
			if (modeFlag == "l" && isAdd == false)
				return false;
		return true;
	}
	return false;
}

bool Server::isRightModeFlag(const std::string &modeFlag)
{
	if (modeFlag == "i" || modeFlag == "t" || modeFlag == "k" || modeFlag == "l" || modeFlag == "o")
		return true;
	return false;
}

void Server::makeModeVector(std::string modeInput, std::vector<std::pair<char, std::string> > &vec)
{
	char sign = '+';
	for (size_t i = 0; i < modeInput.size(); i++) {
		if (modeInput[i] == '+' || modeInput[i] == '-') {
			sign = modeInput[i];
			continue;
		}
		vec.push_back(std::make_pair(sign, std::string(1, modeInput[i])));
	}
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

void Server::makeChannelVector(std::string str, std::vector<std::string> &vec)
{
	std::string inputChannelName;
	size_t len = str.length();
	size_t pos;
	size_t start = 0;

	for (pos = 0; pos < len; pos++) {
		if (str[pos] == ',') {
			inputChannelName = str.substr(start, pos - start);
			start = pos + 1;
			if (inputChannelName.empty())
				inputChannelName = "#";
			if (find(vec.begin(), vec.end(), inputChannelName) == vec.end())
				vec.push_back(inputChannelName);
		}
		else if (pos == len - 1) {
			inputChannelName = str.substr(start, pos - start + 1);
			if (find(vec.begin(), vec.end(), inputChannelName) == vec.end())
				vec.push_back(inputChannelName);
		}
	}
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
		response = Response::success(RPL_TOPIC, channelName, this->name, nickname, topic);
		send(fd, response.c_str(), response.length(), 0);
	}
	// name list
	std::string nameList;
	std::map<int, Client *> members = channel->getMembers();
	std::map<int, Client *>::iterator iter = members.begin();
	for (; iter != members.end(); iter++) {
		if (channel->isOperator(iter->first)) {
			nameList += "@";
		}
		nameList += iter->second->getNickname() + " ";
	}
	response = Response::success(RPL_NAMREPLY, channelName, this->name, nickname, nameList);
	send(fd, response.c_str(), response.length(), 0);
	response = Response::success(RPL_ENDOFNAMES, channelName, this->name, nickname, "");
	send(fd, response.c_str(), response.length(), 0);
}

std::string Server::extractBonusCommand(const std::string &str)
{
	if (!str.empty() && str[0] == '!') {
		std::string tmp = str.substr(1);
		makeUpper(tmp);
		return tmp;
	}
	return "";
}

ErrorCode Server::privmsgToChannel(const std::string &channelName, const std::string &message, int fd)
{
	if (!isValidChannelName(channelName))
		return ERR_BADCHANNAME;
	if (this->channels.find(channelName) == this->channels.end())
		return ERR_NOSUCHCHANNEL;
	if (!this->channels[channelName]->isMember(fd))
		return ERR_NOTONCHANNEL;
	
	Channel *channel = this->channels[channelName];
	std::map<int, Client *> members = channel->getMembers();
	std::map<int, Client *>::iterator iter = members.begin();
	std::string prefix = this->clients[fd]->getPrefix();
	std::string response;

	for (; iter != members.end(); iter++) {
		if (iter->first == fd)
			continue;
		response = Response::customMessageForPrivmsg(prefix, channelName, message);
		send(iter->first, response.c_str(), response.length(), 0);
	}
	return ERR_NONE;
}

ErrorCode Server::privmsgToUser(const std::string &nickname, const std::string &message, int fd)
{
	if (!isClientInServer(nickname))
		return ERR_NOSUCHNICK;

	Client *client = getClientByNickname(nickname);
	std::string prefix = this->clients[fd]->getPrefix();
	std::string response = Response::customMessageForPrivmsg(prefix, nickname, message);
	send(client->getClientFd(), response.c_str(), response.length(), 0);
	return ERR_NONE;
}