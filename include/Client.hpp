#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client {
	private:
		int client_fd;
		std::string host;
		std::string user_name;
		std::string nickname;
		std::string real_name;
		bool auth;
		bool is_registered;
	public:
		Client();
		Client(const int&, const std::string&);
		Client(const Client&);
		Client &operator=(const Client&);
		~Client();
		void setAuth(bool);
		void setIsRegistered(bool);
};

#endif