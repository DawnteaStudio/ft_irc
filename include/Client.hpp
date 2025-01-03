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
		bool isValidPwd;
		bool isRegistered;
	public:
		Client();
		Client(const int&, const std::string&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setIsValidPwd(bool);
		void setIsRegistered(bool);
		const std::string &getNickname() const;
		const bool &getIsValidPwd() const;
		const bool &getIsRegistered() const;
};

#endif