#include "../include/Server.hpp"

ErrorCode Server::modeO(const std::string &channelName, const std::string &param, bool isAdd)
{
	if (param == "")
		return ERR_NEEDMOREPARAMS;
	Client *client = this->getClientByNickname(param);
	Channel *channel = this->channels[channelName];

	// +o 에서 채널에 속하지 않은 사용자를 오퍼레이터로 만들려고 할 때 어떤 에러가 발생하는 지 확인해야 함
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

	// +k 는 비밀번호를 맞추지 않아도 새롭게 세팅이 가능한 지 확인해야 함
	channel->setIsKeyRequired(isAdd);
	if (isAdd)
		channel->setKey(param);
	else {
		if (channel->getKey() != param)
			return ERR_BADCHANNELKEY;
		channel->setKey("");
	}
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
	int limit;
	try {
		limit = std::stoi(param);
	}
	catch(const std::exception& e) {
		limit = 0;
	}
	if (limit < 0)
		limit = 0;
	if (isAdd)
		channel->setLimit(limit);
	return ERR_NONE;
}

void Server::updateModes(bool isAdd, char mode, Channel *channel, const std::string &param)
{
	int paramSize = channel->getModeParams().size();

	if (isAdd) {
		if (find(channel->getModes().begin(), channel->getModes().end(), mode) == channel->getModes().end())
			channel->getModes().push_back(mode);
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
		channel->getModes().erase(remove(channel->getModes().begin(), channel->getModes().end(), mode), channel->getModes().end());
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