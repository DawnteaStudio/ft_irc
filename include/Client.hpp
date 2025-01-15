#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include "Server.hpp"

class Client {
	private:
		int clientFd;
		std::string userName;
		std::string nickname;
		std::string realName;
		std::string prefix;
		std::string ipAddr;
		std::string buffer;
		bool isValidPasswd;
		bool isRegistered;
		std::vector<Channel *> channels;
		Client();
	public:
		Client(const int&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setIsValidPasswd(bool);
		void setIsRegistered(bool);
		void setNickname(const std::string&);
		void setUserName(const std::string&);
		void setRealName(const std::string&);
		void setPrefix();
		void appendBuffer(const std::string&);
		void clearBuffer();
		void setIpAddr(const std::string&);
		void addChannel(Channel *);
		void removeChannel(Channel *);
		const std::string &getNickname() const;
		const std::string &getUserName() const;
		const std::string &getRealName() const;
		const std::string &getBuffer() const;
		const bool &getIsValidPasswd() const;
		const bool &getIsRegistered() const;
		const std::string &getPrefix() const;
		const int &getClientFd() const;
		std::vector<Channel *> getChannels() const;
};

#endif