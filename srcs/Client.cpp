#include "../include/Client.hpp"

Client::Client() : clientFd(0), isValidPasswd(false), isRegistered(false), gameMode(false) {}

Client::Client(const int &clientFd) : clientFd(clientFd), isValidPasswd(false), isRegistered(false), gameMode(false), isFirstLogin(true) {}

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
		this->realName = other.realName;
		this->prefix = other.prefix;
		this->ipAddr = other.ipAddr;
		this->isValidPasswd = other.isValidPasswd;
		this->isRegistered = other.isRegistered;
		this->channels = other.channels;
		this->gameMode = other.gameMode;
	}
	return *this;
}

void Client::setIsValidPasswd(bool isValidPwd) { this->isValidPasswd = isValidPwd; }

void Client::setIsRegistered(bool isRegistered) { this->isRegistered = isRegistered; }

void Client::setNickname(const std::string &newNickname) { this->nickname = newNickname; }

void Client::setUserName(const std::string &newUserName) { this->userName = newUserName; }

void Client::setRealName(const std::string &newRealName) { this->realName = newRealName; }

void Client::setPrefix() { this->prefix = this->nickname + "!" + this->userName + "@" + this->ipAddr; }

void Client::setIpAddr(const std::string &newIpAddr) { this->ipAddr = newIpAddr; }

void Client::setGameMode(bool gameMode) { this->gameMode = gameMode; }

void Client::setHp(int hp) { this->hp = hp; }

void Client::setIsFirstLogin(bool isFirstLogin) { this->isFirstLogin = isFirstLogin; }

const int &Client::getClientFd() const { return this->clientFd; }

const int &Client::getHp() const { return this->hp; }

const int &Client::getHighScore() const { return this->highScore; }

const int &Client::getPlayingScore() const { return this->playingScore; }

void Client::appendBuffer(const std::string &newBuffer) { this->buffer += newBuffer; }

void Client::setBuffer(const std::string &newBuffer) { this->buffer = newBuffer; }

void Client::clearBuffer() { this->buffer.clear(); }

void Client::addChannel(Channel *channel) { this->channels.push_back(channel); }

void Client::addInvitedChannel(const std::string &channelName) { this->invitedChannels.push_back(channelName); }

void Client::removeInvitedChannel(const std::string &channelName)
{
	std::vector<std::string>::iterator it = std::find(this->invitedChannels.begin(), this->invitedChannels.end(), channelName);
	if (it != this->invitedChannels.end())
		this->invitedChannels.erase(it);
}

const std::string &Client::getNickname() const { return this->nickname; }

const bool &Client::getIsValidPasswd() const { return this->isValidPasswd; }

const bool &Client::getIsRegistered() const { return this->isRegistered; }

bool Client::isInvitedChannel(const std::string &channelName) const
{
	std::vector<std::string>::const_iterator it = std::find(this->invitedChannels.begin(), this->invitedChannels.end(), channelName);
	if (it != this->invitedChannels.end())
		return true;
	return false;
}

bool Client::getGameMode() const { return this->gameMode; }

bool Client::getIsFirstLogin() const { return this->isFirstLogin; }

const std::string &Client::getUserName() const { return this->userName; }

const std::string &Client::getRealName() const { return this->realName; }

const std::string &Client::getBuffer() const { return this->buffer; }

const std::string &Client::getPrefix() const { return this->prefix; }

const std::string &Client::getIpAddr() const { return this->ipAddr; }

std::vector<Channel *> Client::getChannels() const { return (this->channels); }

void Client::damageHp(int damage)
{
	this->hp -= damage;
	if (this->hp < 0)
		this->hp = 0;
}

void Client::setHighScore(int highScore) { this->highScore = highScore; }

void Client::setPlayingScore(int playingScore) { this->playingScore = playingScore; }

void Client::takeScore(int score) { this->playingScore += score; }

void Client::removeChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = std::find(this->channels.begin(), this->channels.end(), channel);
	if (it != this->channels.end())
		this->channels.erase(it);
}