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

std::string Response::success(const int &num, const std::string &channelName, const std::string &prefix, const std::string &clientNickname, const std::string &param)
{
	std::string res;

	if (num == RPL_NOTOPIC)
		res = channelName + " :No topic is set";
	else if (num == RPL_TOPIC)
		res = channelName + " :" + param;
	else if (num == RPL_NAMREPLY)
		res = channelName + " :" + param;
	else if (num == RPL_ENDOFNAMES)
		res = channelName + " :End of /NAMES list";
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
	else if (num == ERR_UNKNOWNCOMMAND)
		res = param + " :Unknown command";
	return createMessage(num, res, prefix, clientNickname);
}

std::string Response::createMessage(const int &num, const std::string &res, const std::string &prefix, const std::string &clientNickname)
{
	std::string tmp = clientNickname;
	if (tmp == "")
		tmp = '*';
	return ":" + prefix + " " + std::to_string(num) + " " + tmp + " " + res + CRLF;
}

std::string Response::customMessageForJoin(const std::string &prefix, const std::string &channelName)
{
	return ":" + prefix + " JOIN :" + channelName + CRLF;
}

std::string Response::customMessageForKick(const std::string &prefix, const std::string &channelName, const std::string &target, const std::string &reason)
{
	return ":" + prefix + " KICK " + channelName + " " + target + " :" + reason + CRLF;
}

std::string Response::customMessageForPart(const std::string &prefix, const std::string &channelName)
{
	return ":" + prefix + " PART :" + channelName + CRLF;
}

std::string Response::customMessageForQuit(const std::string &prefix, const std::string &reason)
{
	return ":" + prefix + " QUIT :" + reason + CRLF;
}

std::string Response::customErrorMessageForQuit(const std::string &user, const std::string &reason)
{
	return "ERROR :Closing link: (" + user + ") [" + reason + "]" + CRLF;
}

std::string Response::customMessageForPrivmsg(const std::string &prefix, const std::string &target, const std::string &message)
{
	return ":" + prefix + " PRIVMSG " + target + " " + target + " :" + message + CRLF;
}

std::string Response::customMessageForInvite(const std::string &prefix, const std::string &channelName, const std::string &target)
{
	return ":" + prefix + " INVITE " + target + " :" + channelName + CRLF;
}