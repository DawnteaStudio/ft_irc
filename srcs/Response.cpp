#include "../include/Response.hpp"

Response::Response() {}

Response::~Response() {}

Response::Response(const Response &other)
{
	(void)other;
}

Response &Response::operator=(const Response &other)
{
	(void)other;
	return *this;
}

std::string Response::success(const int &num, const std::string &param, const std::string &prefix, const std::string &clientNickname)
{
	std::string res;

	if (num == RPL_YOUREOPER)
		res = ":You are now an IRC operator";
	return createMessage(num, res, prefix, clientNickname);
}

std::string Response::failure(const int &num, const std::string &param, const std::string &prefix, const std::string &clientNickname)
{
	std::string res;

	if (num == ERR_NEEDMOREPARAMS)
		res = param + " :Not enough parameters";
	else if (num == ERR_ALREADYREGISTRED)
		res = ":You may not reregister";
	else if (num == ERR_NONICKNAMEGIVEN)
		res = ":No nickname given";
	else if (num == ERR_ERRONEUSNICKNAME)
		res = param + " :Erroneus nickname";
	else if (num == ERR_NICKNAMEINUSE)
		res = param + " :Nickname is already in use";
	else if (num == ERR_PASSWDMISMATCH)
		res = ":Password incorrect";
	else if (num == ERR_NOTREGISTERED)
		res = ":You have not registered";
	return createMessage(num, res, prefix, clientNickname);
}

std::string Response::createMessage(const int &num, const std::string &res, const std::string &prefix, const std::string &clientNickname)
{
	std::string tmp = clientNickname;
	if (tmp == "")
		tmp = '*';
	return ":" + prefix + " " + std::to_string(num) + " " + tmp + " " + res;
}