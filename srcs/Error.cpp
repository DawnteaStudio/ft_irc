#include "include/Error.hpp"

Error::Error() {}

Error::~Error() {}

Error::Error(const Error &other)
{
	(void)other;
}

Error &Error::operator=(const Error &other)
{
	(void)other;
	return *this;
}

std::string &Error::createErrorMessage(const int &num, const std::string &param)
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
	return res;
}