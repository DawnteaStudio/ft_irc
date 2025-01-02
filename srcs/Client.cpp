#include "../include/Client.hpp"

Client::Client() : client_fd(0), host(), user_name(), nickname(), auth(false), is_registered(false) {}

Client::Client(const int &client_fd, const std::string &host) : client_fd(client_fd), host(host), user_name(), nickname(), auth(false), is_registered(false) {}

Client::~Client() {}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other) {
		this->client_fd = other.client_fd;
		this->host = other.host;
		this->user_name = other.user_name;
		this->nickname = other.nickname;
		this->auth = other.auth;
		this->is_registered = other.is_registered;
	}
	return *this;
}

void Client::setAuth(bool auth) { this->auth = auth; }
void Client::setIsRegistered(bool is_registered) { this->is_registered = is_registered; }