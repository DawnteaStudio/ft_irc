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
		return customMessageForChannelTopic(num, prefix, channelName, clientNickname, param);
	else if (num == RPL_NAMREPLY)
		return customMessageForNamelist(num, prefix, channelName, clientNickname, param);
	else if (num == RPL_ENDOFNAMES)
		return customMessageForEndOfName(num, prefix, channelName, clientNickname);
	else if (num == RPL_INVITING)
		res = channelName + " " + param;
	else if (num == RPL_CHANNELMODEIS)
		res = channelName + " " + param;
	else if (num == RPL_WELCOME) {
		res = ": 001 " + clientNickname + " : Welcome to ft_irc SERVER!" + CRLF;
		return res;
	}
	else if (num == RPL_CHANGEDNICK) {
		res = ":" + prefix + " NICK :" + param + CRLF;
		return res;
	}
	else if (num == RPL_PONG) {
		res = ":" + channelName + " PONG " + param + " :" + channelName + CRLF;
		return res;
	}
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
	else if (num == ERR_NOTREGISTERED)
		res = ":You have not registered";
	else if (num == ERR_UNKNOWNCOMMAND)
		res = param + " :Unknown command";
	else if (num == ERR_FILEERROR)
		res = param + " :File error";
	else if (num == ERR_NOTINGAME)
		res = ":You are not in game";
	else if (num == ERR_ALREADYINGAME)
		res = ":You are already in game";
	else if (num == ERR_NOSUCHCHANNEL) //no such channel error 추가
		res = param + " :No such channel";
	else if (num == ERR_NOTONCHANNEL) //not on channel error 추가
		res = param + " :You're not on that channel";
	else if (num == ERR_CHANOPRIVSNEEDED) //channel operator privileges needed error 추가
		res = param + " :You're not a channel operator";
	else if (num == ERR_UNKNOWNMODE) //unknown mode error 추가
		res = param + " :Unknown mode";
	else if (num == ERR_INVITEONLYCHAN) //invite only channel error 추가
		res = param + " :Invite only channel";
	else if (num == ERR_BADCHANNELKEY) //bad channel key error 추가
		res = param + " :Bad channel key";
	else if (num == ERR_BADCHANNAME) //bad channel name error 추가
		res = param + " :Bad channel name";
	else if (num == ERR_INVALIDFILEPATH) //invalid file path error 추가
		res = param + " :Invalid file path";
	else if (num == ERR_TOOMANYCHANNELS) //too many channels error 추가
		res = param + " :Too many channels";
	else if (num == ERR_USERONCHANNEL) //user on channel error 추가
		res = param + " :User on channel";
	else if (num == ERR_NOSUCHNICK)
		res = param + " :No such nick";
	else if (num == ERR_KEYSET)
		res = param + " :Channel key already set";
	else if (num == ERR_CHANNELISFULL)
		res = param + " :Cannot join channel (channel is full)";
	else if (num == ERR_FILENOTFOUND)
		res = param + " :File not found";
	
	return createMessage(num, res, prefix, clientNickname);
}

std::string Response::createMessage(const int &num, const std::string &res, const std::string &prefix, const std::string &clientNickname)
{
	std::string tmp = clientNickname;
	if (tmp == "")
		tmp = '*';

	std::ostringstream oss;
	oss << num;
	return ":" + prefix + " " + oss.str() + " " + tmp + " " + res + CRLF;
}

std::string Response::customMessageForJoin(const std::string &prefix, const std::string &channelName)
{
	return ":" + prefix + " JOIN :" + channelName + CRLF;
}

// ":irc.localhost 353 " + nickname + " = " + channelName + " :" + nameList + CRLF;
std::string Response::customMessageForChannelTopic(const int &num, const std::string &prefix, const std::string &channelName, const std::string &nickname, const std::string &topic)
{
	std::ostringstream oss;
	oss << num;
	return ":" + prefix + " " + oss.str() + nickname + " " + channelName + " :" + topic + CRLF;
}

std::string Response::customMessageForNamelist(const int &num, const std::string &prefix, const std::string &channelName, const std::string &nickname, const std::string &nameList)
{
	std::ostringstream oss;
	oss << num;
	return ":" + prefix + " " + oss.str() + " " + nickname + " = " + channelName + " :" + nameList + CRLF;
}

std::string Response::customMessageForEndOfName(const int &num, const std::string &prefix, const std::string &channelName, const std::string &nickname)
{
	std::ostringstream oss;
	oss << num;
	return ":" + prefix + " " + oss.str() + " " + nickname + " " + channelName + " " + "End of /NAMES list." + CRLF;
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
	return ":" + prefix + " PRIVMSG " + target + " :" + message + CRLF;
}

std::string Response::customMessageForInvite(const std::string &prefix, const std::string &channelName, const std::string &target)
{
	return ":" + prefix + " INVITE " + target + " :" + channelName + CRLF;
}

std::string Response::customMessageForMode(const std::string &prefix, const std::string &channelName, const std::string &mode)
{
	return ":" + prefix + " MODE " + channelName + " " + mode + CRLF;
}