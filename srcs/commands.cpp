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
	if (isUsedUserNickname(request.args[0]))
		return (createMessage(ERR_NICKNAMEINUSE, this->clients[i]->getNickname(), Response::failure(ERR_NICKNAMEINUSE, request.args[0])));
	if (this->clients[i]->getIsValidPasswd()) {
		if (this->clients[i]->getNickname() != "")
			deleteUserNickname(this->clients[i]->getNickname());
		this->clients[i]->setNickname(request.args[0]);
		addNewUserNickname(request.args[0]);
	}
	return "";
}

std::string Server::setUser(Request &request, int i)
{
	if (request.args.size() != 4)
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Response::failure(ERR_NEEDMOREPARAMS, "USER")));
	if (this->clients[i]->getIsRegistered())
		return (createMessage(ERR_ALREADYREGISTRED, this->clients[i]->getNickname(), Response::failure(ERR_ALREADYREGISTRED, "")));
	if (this->clients[i]->getNickname() != "") {
		this->clients[i]->setUserName(request.args[0]);
		this->clients[i]->setRealName(request.args[3]);
		this->clients[i]->setIsRegistered(true);
	}
	return "";
}

std::string Server::setOper(Request &request, int i)
{
	if (request.args.size() != 2)
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Response::failure(ERR_NEEDMOREPARAMS, "OPER")));
	if (request.args[1] != this->password)
		return (createMessage(ERR_PASSWDMISMATCH, this->clients[i]->getNickname(), Response::failure(ERR_PASSWDMISMATCH, "")));
	this->clients[i]->setIsOperator(true);
	return (createMessage(RPL_YOUREOPER, this->clients[i]->getNickname(), Response::success(RPL_YOUREOPER, "")));
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