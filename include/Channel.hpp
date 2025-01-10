#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Server.hpp"

class Client;

class Channel {
	private:
		std::string name;
		std::string topic;
		std::map<int, Client *> members;
		std::map<int, Client *> operators;
		Channel();
	public:
		Channel(const std::string&);
		Channel(const Channel&);
		Channel &operator=(const Channel&);
		~Channel();
		void addMember(Client *);
		void removeMember(int);
		void addOperator(Client *);
		void removeOperator(int);
};
#endif
