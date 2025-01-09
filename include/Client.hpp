#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include "Server.hpp"

class Client {
	private:
		int clientFd;
		std::string hostName;
		std::string serverName;
		std::string userName;
		std::string nickname;
		std::string realName;
		std::string buffer;
		bool isValidPasswd;
		bool isRegistered;
		std::vector<Channel *> channels;
		bool isOperator;
		Client();
	public:
		Client(const int&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setHostName(const std::string&);
		void setServerName(const std::string&);
		void setIsValidPasswd(bool);
		void setIsRegistered(bool);
		void setNickname(const std::string&);
		void setUserName(const std::string&);
		void setRealName(const std::string&);
		void setIsOperator(bool);
		void appendBuffer(const std::string&);
		void clearBuffer();
		const std::string &getNickname() const;
		const std::string &getUserName() const;
		const std::string &getRealName() const;
		const std::string &getBuffer() const;
		const bool &getIsValidPasswd() const;
		const bool &getIsRegistered() const;
		const bool &getIsOperator() const;
		std::vector<Channel *> getChannels() const;
};

#endif