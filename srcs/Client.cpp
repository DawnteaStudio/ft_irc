#include "../include/Client.hpp"

Client::Client() : clientFd(0), isValidPasswd(false), isRegistered(false), isOperator(false) {}

Client::Client(const int &clientFd) : clientFd(clientFd), isValidPasswd(false), isRegistered(false), isOperator(false) {}

Client::~Client() {}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other) {
		this->clientFd = other.clientFd;
		this->userName = other.userName;
		this->nickname = other.nickname;
		this->isValidPasswd = other.isValidPasswd;
		this->isRegistered = other.isRegistered;
		this->isOperator = other.isOperator;
	}
	return *this;
}

void Client::setHostName(const std::string &newHostName) { this->hostName = newHostName; }

void Client::setServerName(const std::string &newServerName) { this->serverName = newServerName; }

void Client::setIsValidPasswd(bool isValidPwd) { this->isValidPasswd = isValidPwd; }

void Client::setIsRegistered(bool isRegistered) { this->isRegistered = isRegistered; }

void Client::setNickname(const std::string &newNickname) { this->nickname = newNickname; }

void Client::setUserName(const std::string &newUserName) { this->userName = newUserName; }

void Client::setRealName(const std::string &newRealName) { this->realName = newRealName; }

void Client::setIsOperator(bool isOperator) { this->isOperator = isOperator; }

void Client::appendBuffer(const std::string &newBuffer) { this->buffer += newBuffer; }

void Client::clearBuffer() { this->buffer.clear(); }

const std::string &Client::getNickname() const { return this->nickname; }

const bool &Client::getIsValidPasswd() const { return this->isValidPasswd; }

const bool &Client::getIsRegistered() const { return this->isRegistered; }

const std::string &Client::getUserName() const { return this->userName; }

const std::string &Client::getRealName() const { return this->realName; }

const std::string &Client::getBuffer() const { return this->buffer; }

const bool &Client::getIsRegistered() const { return this->isRegistered; }

std::vector<Channel *> Client::getChannels() const { return (this->channels); }

const bool &Client::getIsOperator() const { return this->isOperator; }
