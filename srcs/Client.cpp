#include "../include/Client.hpp"

Client::Client() : clientFd(0), host(), userName(), nickname(), isValidPasswd(false), isRegistered(false) {}

Client::Client(const int &clientFd, const std::string &host) : clientFd(clientFd), host(host), userName(), nickname(), isValidPasswd(false), isRegistered(false) {}

Client::~Client() {}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other) {
		this->clientFd = other.clientFd;
		this->host = other.host;
		this->userName = other.userName;
		this->nickname = other.nickname;
		this->isValidPasswd = other.isValidPasswd;
		this->isRegistered = other.isRegistered;
	}
	return *this;
}

void Client::setIsValidPasswd(bool IsValidPwd) { this->isValidPasswd = IsValidPwd; }

void Client::setIsRegistered(bool is_registered) { this->isRegistered = is_registered; }

void Client::setNickname(const std::string &newNickname) { this->nickname = newNickname; }

const std::string &Client::getNickname() const { return this->nickname; }

const bool &Client::getIsValidPasswd() const { return this->isValidPasswd; }

const bool &Client::getIsRegistered() const {return this->isRegistered; }