#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>

class Request {
	public:
		std::vector<std::string> args;
		std::string command;
		bool invalidMessage;
	public:
		Request();
		Request(const Request&);
		Request &operator=(const Request&);
		~Request();
};

#endif