#include <algorithm>
#include <cctype>
#include "../include/Server.hpp"

std::string Server::setPassword(Request &request, int i)
{
	if (request.args.empty())
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Response::failure(ERR_NEEDMOREPARAMS, "PASS")));
	if (this->clients[i]->getIsRegistered())
		return (createMessage(ERR_ALREADYREGISTRED, this->clients[i]->getNickname(), Response::failure(ERR_ALREADYREGISTRED, "")));
	if (this->password == request.args[0])
		this->clients[i]->setIsValidPasswd(true);
	else
		this->clients[i]->setIsValidPasswd(false);
	return "";
}

std::string Server::setUserNickname(Request &request, int i)
{
	if (request.args.empty())
		return (createMessage(ERR_NONICKNAMEGIVEN, this->clients[i]->getNickname(), Response::failure(ERR_NONICKNAMEGIVEN, "")));
	if (!isValidUserNickname(request.args[0]))
		return (createMessage(ERR_ERRONEUSNICKNAME, this->clients[i]->getNickname(), Response::failure(ERR_ERRONEUSNICKNAME, request.args[0])));
	if (isUsedUserNickname(request.args[0], this->clientNicknames))
		return (createMessage(ERR_NICKNAMEINUSE, this->clients[i]->getNickname(), Response::failure(ERR_NICKNAMEINUSE, request.args[0])));
	if (this->clients[i]->getIsValidPasswd()) {
		if (this->clients[i]->getNickname() != "")
			deleteUserNickname(this->clients[i]->getNickname(), this->clientNicknames);
		this->clients[i]->setNickname(request.args[0]);
		addNewUserNickname(request.args[0], this->clientNicknames);
	}
	return "";
}

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

bool Server::isUsedUserNickname(const std::string &nickname, std::vector<std::string> &nicknames)
{
	std::string tmp = convertChar(nickname);
	std::vector<std::string>::iterator iter = std::find(nicknames.begin(), nicknames.end(), tmp);
	if (iter != nicknames.end())
		return true;
	return false;
}

void Server::deleteUserNickname(const std::string &nickname, std::vector<std::string> &nicknames)
{
	std::string tmp = convertChar(nickname);
	std::vector<std::string>::iterator iter = std::find(nicknames.begin(), nicknames.end(), tmp);
	nicknames.erase(iter);
}

void Server::addNewUserNickname(const std::string &newNickname, std::vector<std::string> &nicknames)
{
	std::string tmp = convertChar(newNickname);
	this->clientNicknames.push_back(tmp);
}