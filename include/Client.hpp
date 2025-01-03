#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
	private:
		int clientFd;
		std::string host;
		std::string userName;
		std::string nickname;
		std::string realName;
		bool isValidPasswd;
		bool isRegistered;
	public:
		Client();
		Client(const int&, const std::string&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setIsValidPasswd(bool);
		void setIsRegistered(bool);
		void setNickname(const std::string&);
		const std::string &getNickname() const;
		const bool &getIsValidPasswd() const;
		const bool &getIsRegistered() const;
};

#endif