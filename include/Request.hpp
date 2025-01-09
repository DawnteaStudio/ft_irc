#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
#include "Server.hpp"

class Request {
	public:
		std::vector<std::string> args;
		IRCCommand command;
		std::string prefix;
		bool invalidMessage;
	public:
		Request();
		Request(const Request&);
		Request &operator=(const Request&);
		~Request();
		void parse(std::string);
		const IRCCommand getCommand() const;
};

#endif