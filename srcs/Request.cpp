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

void Request::parse(std::string &buffer)
{
	if (buffer.empty() || buffer[0] == ' ') {
		this->invalidMessage = true;
		return;
	}
	
	if (this->parsePrefix(buffer))
		return;
	this->parseCommand(buffer);
	this->parseArgs(buffer);
	if (this->args.size() > 15)
		this->invalidMessage = true;
}

bool Request::parsePrefix(std::string &buffer)
{
	std::string::size_type pos;
	if (buffer[0] == ':') {
		pos = buffer.find(' ');
		if (pos == std::string::npos)
			this->invalidMessage = true;
		else {
			this->prefix = buffer.substr(1, pos - 1);
			buffer = buffer.substr(pos + 1);
		}
	}
	pos = buffer.find_first_not_of(" ");
	if (pos != std::string::npos)
		buffer = buffer.substr(pos);
	else
		this->invalidMessage = true;
	return this->invalidMessage;
}

void Request::parseCommand(std::string &buffer)
{
	std::string::size_type pos;
	pos = buffer.find(' ');
	if (pos != std::string::npos) {
		this->command = buffer.substr(0, pos);
		buffer = buffer.substr(pos + 1);
	}
	else {
		this->command = buffer;
		buffer = "";
	}
}

void Request::parseArgs(std::string &buffer)
{
	std::string tail = "";
	std::string::size_type pos = buffer.find(':');
	if (pos != std::string::npos) {
		tail = buffer.substr(pos + 1);
		buffer = buffer.substr(0, pos);
	}
	while (!buffer.empty()) {
		std::string::size_type start = buffer.find_first_not_of(" ");
		if (start == std::string::npos)
			break;

		std::string::size_type end = buffer.find(' ', start);
		if (end != std::string::npos) {
			this->args.push_back(buffer.substr(start, end - start));
			buffer = buffer.substr(end);
		}
		else {
			this->args.push_back(buffer.substr(start));
			break;
		}
	}
	if (!tail.empty())
		this->args.push_back(tail);
}

const std::string &Request::getCommand() const { return this->command; }