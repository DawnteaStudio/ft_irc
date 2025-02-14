#include "../include/Server.hpp"

ErrorCode Server::modeO(const std::string &channelName, const std::string &param, bool isAdd)
{
	if (param == "")
		return ERR_NEEDMOREPARAMS;
	Client *client = this->getClientByNickname(param);
	Channel *channel = this->channels[channelName];

	if (client == NULL || !channel->isMember(client->getClientFd()))
		return ERR_NOSUCHNICK;
	if (isAdd)
		channel->addOperator(client);
	else
		channel->removeOperator(client->getClientFd());
	return ERR_NONE;
}

ErrorCode Server::modeK(const std::string &channelName, const std::string &param, bool isAdd)
{
	if (param == "")
		return ERR_NEEDMOREPARAMS;
	Channel *channel = this->channels[channelName];

	if (isAdd) {
		if (channel->getIsKeyRequired())
			return ERR_DONOTHING; // 이미 키가 설정되어 있는 경우, 아무 동작도 하지 않음
		channel->setKey(param);
	}
	else {
		if (channel->getKey() != param)
			return ERR_KEYSET;
		channel->setKey("");
	}
	channel->setIsKeyRequired(isAdd);
	return ERR_NONE;
}

ErrorCode Server::modeL(const std::string &channelName, const std::string &param, bool isAdd)
{
	Channel *channel = this->channels[channelName];

	if (!isAdd) {
		channel->setIsLimit(isAdd);
		return ERR_NONE;
	}
	if (param == "")
		return ERR_NEEDMOREPARAMS;

	int limit = 0;
	std::istringstream iss(param);
	if (!(iss >> limit))
		limit = 0;
	if (limit < 0)
		limit = 0;

	if (isAdd)
		channel->setLimit(limit);
	return ERR_NONE;
}

void Server::updateModes(bool isAdd, char mode, Channel *channel, const std::string &param)
{
	int paramSize = channel->getModeParams().size();
	std::vector<char> modes = channel->getModeVector();

	if (isAdd) {
		if (find(modes.begin(), modes.end(), mode) == modes.end()) {
			modes.push_back(mode);
			channel->setModeVector(modes);
		}

		if (param != "") {
			for (int i = 0; i < paramSize; i++) {
				if (channel->getModeParams()[i].first == mode) {
					channel->getModeParams()[i].second = param;
					return;
				}
			}
		}
	}
	else {
		if (find(modes.begin(), modes.end(), mode) != modes.end())
			modes.erase(find(modes.begin(), modes.end(), mode));
		channel->setModeVector(modes);
		for (int i = 0; i < paramSize; i++) {
			if (channel->getModeParams()[i].first == mode) {
				channel->getModeParams().erase(channel->getModeParams().begin() + i);
				return;
			}
		}
	}
}

ErrorCode Server::mode(const std::string &channelName, const std::pair<char, std::string> &mode, const std::string &param)
{
	ErrorCode err = ERR_NONE;
	Channel *channel = this->channels[channelName];
	bool isAdd = mode.first == '+';

	switch (mode.second[0]) {
		case 'i':
			channel->setIsInviteOnly(isAdd);
			break;
		case 't':
			channel->setIsTopicChangeByOperatorOnly(isAdd);
			break;
		case 'o':
			err = modeO(channelName, param, isAdd);
			break;
		case 'k':
			err = modeK(channelName, param, isAdd);
			break;
		case 'l':
			err = modeL(channelName, param, isAdd);
			break;
		default:
			break;
	}
	if (err == ERR_NONE)
		updateModes(isAdd, mode.second[0], channel, param);
	return err;
}