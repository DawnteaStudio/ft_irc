#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <algorithm>
# include "Server.hpp"

class Channel;
class Client {
	private:
		int clientFd;
		int hp;
		int highScore;
		int playingScore;
		std::string userName;
		std::string nickname;
		std::string realName;
		std::string prefix;
		std::string ipAddr;
		std::string buffer;
		bool isValidPasswd;
		bool isRegistered;
		bool gameMode;
		std::vector<Channel *> channels;
		std::vector<std::string> invitedChannels;
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
		void setHp(int);
		void setGameMode(bool);
		void setHighScore(int);
		void setPlayingScore(int);
		void takeScore(int);
		void appendBuffer(const std::string&);
		void clearBuffer();
		void setIpAddr(const std::string&);
		void addChannel(Channel *);
		void removeChannel(Channel *);
		void addInvitedChannel(const std::string&);
		void removeInvitedChannel(const std::string&);
		void damageHp(int);
		const std::string &getNickname() const;
		const std::string &getUserName() const;
		const std::string &getRealName() const;
		const std::string &getBuffer() const;
		const bool &getIsValidPasswd() const;
		const bool &getIsRegistered() const;
		bool isInvitedChannel(const std::string&) const;
		bool getGameMode() const;
		const std::string &getPrefix() const;
		const int &getClientFd() const;
		const int &getHp() const;
		const int &getHighScore() const;
		const int &getPlayingScore() const;
		std::vector<Channel *> getChannels() const;
};

#endif