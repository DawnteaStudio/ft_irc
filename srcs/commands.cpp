#include <algorithm>
#include "../include/Server.hpp"

std::string Server::setPassword(Request &request, int i)
{
	if (request.args.empty())
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Error::createErrorMessage(ERR_NEEDMOREPARAMS, "PASS")));
	if (this->clients[i]->getIsRegistered())
		return (createMessage(ERR_ALREADYREGISTRED, this->clients[i]->getNickname(), Error::createErrorMessage(ERR_ALREADYREGISTRED, "")));
	if (this->password == request.args[0])
		this->clients[i]->setIsValidPasswd(true);
	else
		this->clients[i]->setIsValidPasswd(false);
	return "";
}

std::string Server::setUserNickname(Request &request, int i)
{
	if (request.args.empty())
		return (createMessage(ERR_NONICKNAMEGIVEN, this->clients[i]->getNickname(), Error::createErrorMessage(ERR_NONICKNAMEGIVEN, "")));
	if (!isValidUserNickname(request.args[0]))
		return (createMessage(ERR_ERRONEUSNICKNAME, this->clients[i]->getNickname(), Error::createErrorMessage(ERR_ERRONEUSNICKNAME, request.args[0])));
	if (isUsedUserNickname(request.args[0], this->clientNicknames))
		return (createMessage(ERR_NICKNAMEINUSE, this->clients[i]->getNickname(), Error::createErrorMessage(ERR_NICKNAMEINUSE, request.args[0])));
	if (this->clients[i]->getIsValidPasswd()) {
		if (this->clients[i]->getNickname() != "")
			deleteUserNickname(this->clients[i]->getNickname(), this->clientNicknames);
		this->clients[i]->setNickname(request.args[0]);
		this->clientNicknames.push_back(request.args[0]);
	}
	return "";
}

bool isValidUserNickname(const std::string &nickname)
{
	// parsing nickname
	return true;
}

bool isUsedUserNickname(const std::string &nickname, std::vector<std::string> &nicknames)
{
	std::vector<std::string>::iterator iter = std::find(nicknames.begin(), nicknames.end(), nickname);
	if (iter != nicknames.end())
		return true;
	return false;
}

void deleteUserNickname(const std::string &nickname, std::vector<std::string> &nicknames)
{
	std::vector<std::string>::iterator iter = std::find(nicknames.begin(), nicknames.end(), nickname);
	nicknames.erase(iter);
}
