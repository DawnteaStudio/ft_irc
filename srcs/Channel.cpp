#include "../include/Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &name) : name(name), limit(10), isInviteOnly(false), isTopicChangeByOperatorOnly(true), isKeyRequired(false), isLimit(false)
{
	this->modes.push_back('t');
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other) {
		this->isKeyRequired = other.isKeyRequired;
		this->isInviteOnly = other.isInviteOnly;
		this->isTopicChangeByOperatorOnly = other.isTopicChangeByOperatorOnly;
		this->isLimit = other.isLimit;
		this->key = other.key;
		this->limit = other.limit;
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

void Channel::removeMember(int fd) { this->members.erase(fd); }

void Channel::addOperator(Client *client)
{
	int fd = client->getClientFd();
	this->operators[fd] = client;
}

void Channel::removeOperator(int fd) { this->operators.erase(fd); }

void Channel::addFile(std::string &fileName, std::string &content)
{
	File file(fileName, this->name);
	file.setFileContent(content);
	this->files[fileName] = file;
}

void Channel::addInvitedClient(int fd) { this->invitedClients.push_back(fd); }

void Channel::removeInvitedClient(int fd)
{
	std::vector<int>::iterator it = std::find(this->invitedClients.begin(), this->invitedClients.end(), fd);
	if (it != this->invitedClients.end())
		this->invitedClients.erase(it);
}

void Channel::removeFile(const std::string &fileName) { this->files.erase(fileName); }

const std::string &Channel::getName() const { return this->name; }

const std::string &Channel::getKey() const { return this->key; }

const std::string &Channel::getTopic() const { return this->topic; }

const int &Channel::getLimit() const { return this->limit; }

const bool &Channel::getIsInviteOnly() const { return this->isInviteOnly; }

const bool &Channel::getIsTopicChangeByOperatorOnly() const { return this->isTopicChangeByOperatorOnly; }

const bool &Channel::getIsKeyRequired() const { return this->isKeyRequired; }

const bool &Channel::getIsLimit() const { return this->isLimit; }

std::map<int, Client *> Channel::getMembers() const { return this->members; }

std::map<int, Client *> Channel::getOperators() const { return this->operators; }

std::map<std::string, File> Channel::getFiles() const { return this->files; }

bool Channel::findFile(const std::string &fileName)
{
	if (this->files.find(fileName) == this->files.end())
		return false;
	return true;
}

std::vector<int> Channel::getInvitedClients() const { return this->invitedClients; }

std::vector<char> Channel::getModes() const { return this->modes; }

std::vector<std::pair<char, std::string> > Channel::getModeParams() const { return this->modeParams; }

bool Channel::isMember(int fd) const { return this->members.find(fd) != this->members.end(); }

bool Channel::isOperator(int fd) const { return this->operators.find(fd) != this->operators.end(); }

bool Channel::isInvitedClient(int fd) const { return std::find(this->invitedClients.begin(), this->invitedClients.end(), fd) != this->invitedClients.end(); }

void Channel::setKey(const std::string &key) { this->key = key; }

void Channel::setTopic(const std::string &topic) { this->topic = topic; }

void Channel::setLimit(const int limit)
{
	if (limit < 0)
		this->limit = 0;
	else
		this->limit = limit;
}

void Channel::setIsInviteOnly(const bool isInviteOnly) { this->isInviteOnly = isInviteOnly; }

void Channel::setIsTopicChangeByOperatorOnly(const bool isTopicChangeByOperatorOnly) { this->isTopicChangeByOperatorOnly = isTopicChangeByOperatorOnly; }

void Channel::setIsKeyRequired(const bool isKeyRequired) { this->isKeyRequired = isKeyRequired; }

void Channel::setIsLimit(const bool isLimit) { this->isLimit = isLimit; }