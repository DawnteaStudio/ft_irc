#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"

class Channel {
	private:
		Channel();
		std::string name;
		std::string topic;
		Server &server;
	public:
		Channel(const std::string &name, Client *client, Server &server);
		~Channel();
		void quit(Client *client);
};
#endif
