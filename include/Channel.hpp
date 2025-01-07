#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"

class Server;

class Channel {
	private:
		Channel();
		std::string name;
		std::string topic;
		Server &server;
	public:
		Channel(const std::string &name, Client *client, Server &server);
		~Channel();
};
#endif
