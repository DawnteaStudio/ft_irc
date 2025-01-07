#include "../include/Client.hpp"

Client::Client() : clientFd(0), host(), userName(), nickname(), isValidPwd(false), isRegistered(false) {}

Client::Client(const int &clientFd) : clientFd(clientFd), host(), userName(), nickname(), isValidPwd(false), isRegistered(false) {}

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
		this->isValidPwd = other.isValidPwd;
		this->isRegistered = other.isRegistered;
	}
	return *this;
}

void Client::setIsValidPwd(bool IsValidPwd) { this->isValidPwd = IsValidPwd; }

void Client::setIsRegistered(bool is_registered) { this->isRegistered = is_registered; }

const std::string &Client::getNickname() const { return this->nickname; }

const bool &Client::getIsValidPwd() const { return this->isValidPwd; }

const bool &Client::getIsRegistered() const {return this->isRegistered; }

std::vector<Channel *> Client::getChannels() const { return (this->channels); }
