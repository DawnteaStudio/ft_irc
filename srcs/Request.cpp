#include "../include/Request.hpp"

Request::Request() : invalidMessage(false) {}

Request::~Request() {}

Request::Request(const Request &other)
{
	*this = other;
}

Request &Request::operator=(const Request &other)
{
	if (this != &other) {
		this->args = other.args;
		this->command = other.command;
		this->invalidMessage = other.invalidMessage;
	}
	return *this;
}

void Request::parse(std::string buffer)
{
	if (buffer.empty() || buffer[0] == ' ') {
		this->invalidMessage = true;
		return;
	}

	if (buffer[0] == ':') {
		std::string::size_type pos = buffer.find(' ');
		if (pos == std::string::npos) {
			this->invalidMessage = true;
			return;
		}
		this->prefix = buffer.substr(1, pos - 1);
		buffer = buffer.substr(pos + 1);
	}
	std::string::size_type start = buffer.find_first_not_of(" ");
	if (start != std::string::npos)
		buffer = buffer.substr(start);
	else {
		this->invalidMessage = true;
		return;
	}

	std::string::size_type pos = buffer.find(' ');
	if (pos != std::string::npos) {
		std::string strCommand = buffer.substr(0, pos);
		// this->command = ;
		buffer = buffer.substr(pos + 1);
	}
	else {
		this->command = buffer;
		buffer.clear();
	}
}

const IRCCommand Request::getCommand() const { return this->command; }