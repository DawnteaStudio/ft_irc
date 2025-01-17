#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "File.hpp"
# include "Server.hpp"

class Client;

class Channel {
	private:
		std::string name;
		std::string key;
		std::string topic;
		int limit;
		bool isInviteOnly;
		bool isTopicChangeByOperatorOnly;
		bool isKeyRequired;
		bool isLimit;
		std::map<int, Client *> members;
		std::map<int, Client *> operators;
		std::map<std::string, File> files;
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
		const std::string &getName() const;
		const std::string &getKey() const;
		const std::string &getTopic() const;
		const int &getLimit() const;
		const bool &getIsInviteOnly() const;
		const bool &getIsTopicChangeByOperatorOnly() const;
		const bool &getIsKeyRequired() const;
		const bool &getIsLimit() const;
		bool isMember(int) const;
		bool isOperator(int) const;
		std::map<int, Client *> getMembers() const;
		std::map<int, Client *> getOperators() const;
		std::map<std::string, File> getFiles() const;
		std::map<std::string, File>::iterator findFile(const std::string&);
		void setKey(const std::string&);
		void setTopic(const std::string&);
		void setLimit(const int);
		void setIsInviteOnly(const bool);
		void setIsTopicChangeByOperatorOnly(const bool);
		void setIsKeyRequired(const bool);
		void setIsLimit(const bool);
};
#endif
