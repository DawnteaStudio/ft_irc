#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include "Server.hpp"

class Client {
	private:
		int clientFd;
		std::string host;
		std::string userName;
		std::string nickname;
		std::string realName;
		bool isValidPwd;
		bool isRegistered;
		std::vector<Channel *> channels;
	public:
		Client();
		Client(const int&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setIsValidPwd(bool);
		void setIsRegistered(bool);
		const std::string &getNickname() const;
		const bool &getIsValidPwd() const;
		const bool &getIsRegistered() const;
		std::vector<Channel *> getChannels() const;
};

#endif