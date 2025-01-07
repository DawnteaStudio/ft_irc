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