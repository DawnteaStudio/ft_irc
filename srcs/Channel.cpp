#include "../include/Channel.hpp"

Channel::Channel(const std::string &name, Client *client, Server &server) : name(name), server(server) {}

Channel::~Channel() {}
