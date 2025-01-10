#include "../include/Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &name) : name(name) {}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other) {
		this->name = other.name;
		this->topic = other.topic;
		this->members = other.members;
		this->operators = other.operators;
	}
	return *this;
}

Channel::~Channel() {}

void Channel::addMember(Client *client)
{
	int fd = client->getClientFd();
	this->members[fd] = client;
}

void Channel::removeMember(int fd)
{
	this->members.erase(fd);
}

void Channel::addOperator(Client *client)
{
	int fd = client->getClientFd();
	this->operators[fd] = client;
}

void Channel::removeOperator(int fd)
{
	this->operators.erase(fd);
}