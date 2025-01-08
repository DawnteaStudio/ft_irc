#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
#include "Server.hpp"

enum IRCCommand {
	JOIN,
	NICK,
	USER,
	PASS,
	CAP,
	MODE,
	KICK,
	PING,
	PONG,
	INVITE,
	PRIVMSG,
	QUIT,
	TOPIC,
	PART,
	WHO,
	WHOIS,
	ERROR,
};

class Request {
	public:
		std::vector<std::string> args;
		std::string command;
		std::string buffer;
		bool invalidMessage;
	public:
		Request();
		Request(const Request&);
		Request &operator=(const Request&);
		~Request();
};

#endif