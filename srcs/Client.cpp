#include "../include/Client.hpp"

Client::Client() : clientFd(0), userName(), nickname(), isValidPasswd(false), isRegistered(false), isOperator(false) {}

Client::Client(const int &clientFd, const std::string &host) : clientFd(clientFd), userName(), nickname(), isValidPasswd(false), isRegistered(false), isOperator(false) {}

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

void Client::setIsValidPasswd(bool isValidPwd) { this->isValidPasswd = isValidPwd; }

void Client::setIsRegistered(bool isRegistered) { this->isRegistered = isRegistered; }

void Client::setNickname(const std::string &newNickname) { this->nickname = newNickname; }

void Client::setUserName(const std::string &newUserName) { this->userName = newUserName; }

void Client::setRealName(const std::string &newRealName) { this->realName = newRealName; }

void Client::setIsOperator(bool IsOperator) { this->isOperator = isOperator; }

const std::string &Client::getNickname() const { return this->nickname; }

const bool &Client::getIsValidPasswd() const { return this->isValidPasswd; }

const bool &Client::getIsRegistered() const {return this->isRegistered; }

const std::string &Client::getUserName() const { return this->userName; }

const std::string &Client::getRealName() const { return this->realName; }

const bool &Client::getIsOperator() const {return this->isOperator; }