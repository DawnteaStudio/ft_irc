#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
#include "Server.hpp"

class Request {
	public:
		std::vector<std::string> args;
		std::string command;
		std::string prefix;
		std::string tail;
		bool invalidMessage;
	public:
		Request();
		Request(const Request&);
		Request &operator=(const Request&);
		~Request();
		void parse(std::string&);
		bool parsePrefix(std::string&);
		void parseCommand(std::string&);
		void parseArgs(std::string&);
		const std::string &getCommand() const;
};

#endif