#include "../include/Server.hpp"

void Server::updateHighScore(int fd)
{
	this->clients[fd]->setGameMode(false);
	if (this->clients[fd]->getPlayingScore() > this->clients[fd]->getHighScore())
		this->clients[fd]->setHighScore(this->clients[fd]->getPlayingScore());
}

std::string Server::winMsg(int fd)
{
	std::string art = GREEN;
	art += R"(
__   __           __        ___       
\ \ / /__  _   _  \ \      / (_)_ __  
 \ V / _ \| | | |  \ \ /\ / /| | '_ \ 
  | | (_) | |_| |   \ V  V / | | | | |
  |_|\___/ \__,_|    \_/\_/  |_|_| |_|
    )";
	art += RESET;
	updateHighScore(fd);
	return art;
}

std::string Server::loseMsg(int fd)
{
	std::string art = RED;
	art += R"(
__   __            _                   
\ \ / /__  _   _  | |    ___  ___  ___ 
 \ V / _ \| | | | | |   / _ \/ __|/ _ \
  | | (_) | |_| | | |__| (_) \__ \  __/
  |_|\___/ \__,_| |_____\___/|___/\___|
    )";
	art += RESET;
	updateHighScore(fd);
	return art;
}

std::string Server::botQuit(int fd)
{
	std::string art = R"(
  ____                 _   ____             
 / ___| ___   ___   __| | | __ ) _   _  ___ 
| |  _ / _ \ / _ \ / _` | |  _ \| | | |/ _ \
| |_| | (_) | (_) | (_| | | |_) | |_| |  __/
 \____|\___/ \___/ \__,_| |____/ \__, |\___|
                                 |___/      
)";
	if (!this->clients[fd]->getGameMode())
		return art;
	updateHighScore(fd);
	this->clients[fd]->setGameMode(false);
	return art;
}

std::string Server::botStart(int fd)
{
	if (this->clients[fd]->getGameMode())
		return Response::failure(ERR_ALREADYINGAME, "", this->name, this->clients[fd]->getNickname());

	std::string art = R"(
__        __   _                          
\ \      / /__| | ___ ___  _ __ ___   ___ 
 \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \
  \ V  V /  __/ | (_| (_) | | | | | |  __/
   \_/\_/ \___|_|\___\___/|_| |_| |_|\___|

YOUR INPUT SHOULD BE IN THE FORM OF "BOT <CHOICE>".
EXAMPLE: BOT ROCK

    BOT ROCK          BOT SCISSORS           BOT PAPER
    _______           _______                _______
---'   ____)      ---'   ____)____       ---'   ____)____
      (_____)               ______)                ______)
      (_____)            __________)               _______)
      (____)            (____)                    _______)
---.__(___)       ---.__(___)            ---.__________)

	)";
	this->clients[fd]->setGameMode(true);
	this->clients[fd]->setPlayingScore(0);
	this->clients[fd]->setHp(5);

	return art;
}

std::string Server::botScore(int fd)
{
	std::string art = BLUE;
	art += "YOUR HIGH SCORE: " + std::to_string(this->clients[fd]->getHighScore()) + "\n";
	if (this->clients[fd]->getGameMode()) {
		art += "YOUR CURRENT SCORE: " + std::to_string(this->clients[fd]->getPlayingScore()) + "\n";
		art += "YOUR REMAINING HP: " + std::to_string(this->clients[fd]->getHp()) + "\n";
	}
	art += RESET;
	return art;
}

std::string Server::botRank(int fd)
{
	std::string art = GOLD;
	art += R"(
  ______      _    _   _ _  _____ _   _  ______  
 / /  _ \    / \  | \ | | |/ /_ _| \ | |/ ___\ \ 
/ /| |_) |  / _ \ |  \| | ' / | ||  \| | |  _ \ \
\ \|  _ <  / ___ \| |\  | . \ | || |\  | |_| |/ /
 \_\_| \_\/_/   \_\_| \_|_|\_\___|_| \_|\____/_/ 
	)";

	std::vector<std::pair<int, std::string>> rank;
	std::map<int, Client *>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); it++)
		rank.push_back(std::make_pair(it->second->getHighScore(), it->second->getNickname()));
	std::sort(rank.begin(), rank.end(), std::greater<std::pair<int, std::string>>());

	for (size_t i = 0; i < rank.size(); i++) {
		if (rank[i].first == 0 || i == 5)
			break;
		art += std::to_string(i + 1) + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
	}
	if (this->clients[fd]->getHighScore() == 0) {
		art += RED;
		art += "Your rank: Not ranked\n";
	}
	else {
		art += "Your rank: ";
		art += RED;
		for (size_t i = 0; i < rank.size(); i++) {
			if (rank[i].second == this->clients[fd]->getNickname()) {
				art += std::to_string(i + 1);
				break;
			}
		}
		art += "\n";
	}
	art += RESET;
	return art;
}

std::string Server::botIntro(int fd)
{
	std::string guide = BLUE;

	guide += "\n\n\n\n";
    guide += R"(

	YOUR OPPONENT IS A BOT.
	YOU CAN PLAY ROCK PAPER SCISSORS WITH THE BOT.
	YOU CAN CHOOSE BETWEEN ROCK, PAPER, SCISSORS.
	YOU CAN ALSO USE HELP TO GET THIS MESSAGE.
	YOUR INPUT SHOULD BE IN THE FORM OF "BOT <CHOICE>".
	EXAMPLE: BOT ROCK

###########################################################

    BOT ROCK          BOT SCISSORS           BOT PAPER
    _______           _______                _______
---'   ____)      ---'   ____)____       ---'   ____)____
      (_____)               ______)                ______)
      (_____)            __________)               _______)
      (____)            (____)                    _______)
---.__(___)       ---.__(___)            ---.__________)

###########################################################
	
	YOU CAN ALSO USE THE FOLLOWING COMMANDS:
	VIEW GUIDE AGAIN: BOT HELP
	START THE GAME: BOT START
	QUIT THE GAME: BOT QUIT
	VIEW YOUR HIGH SCORE: BOT SCORE
	VIEW RANKING: BOT RANK
)";
	guide += RESET;
	return guide;
}

std::string Server::bot(Request &request, int fd)
{
	if (!this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_NOTREGISTERED, "", this->name, this->clients[fd]->getNickname());
	if (request.args.size() < 1)
		return botIntro(fd);

	std::string cmd = request.args[0];
	if (cmd == "HELP")
		return botIntro(fd);
	else if (cmd == "START")
		return botStart(fd);
	else if (cmd == "QUIT")
		return botQuit(fd);
	else if (cmd == "SCORE")
		return botScore(fd);
	else if (cmd == "RANK")
		return botRank(fd);
	else
		return Response::failure(ERR_UNKNOWNCOMMAND, cmd, this->name, this->clients[fd]->getNickname());
	return "";
}
