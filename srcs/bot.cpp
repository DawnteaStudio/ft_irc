#include <ctime>
#include "../include/Server.hpp"

void Server::updateHighScore(int fd)
{
	this->clients[fd]->setGameMode(false);
	if (this->clients[fd]->getPlayingScore() > this->clients[fd]->getHighScore())
		this->clients[fd]->setHighScore(this->clients[fd]->getPlayingScore());
}

std::string Server::drawMsg(int fd)
{
	std::string art = BLUE;
	art += R"(
 ____  ____      ___        ___ _ 
|  _ \|  _ \    / \ \      / / | |
| | | | |_) |  / _ \ \ /\ / /| | |
| |_| |  _ <  / ___ \ V  V / |_|_|
|____/|_| \_\/_/   \_\_/\_/  (_|_) 

)";
	art += RESET;
	return art;
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
	if (!this->clients[fd]->getGameMode())
		return Response::failure(ERR_NOTINGAME, "", this->name, this->clients[fd]->getNickname());
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

	int sameRank = 1;
	for (size_t i = 0; i < rank.size(); i++) {
		if (rank[i].first == 0 || i == 5)
			break;
		if (i == 0)
			art += "1. " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
		else if (rank[i].first == rank[i - 1].first)
			art += std::to_string(sameRank) + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
		else {
			sameRank = i + 1;
			art += std::to_string(i + 1) + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
		}
	}
	if (this->clients[fd]->getHighScore() == 0) {
		art += RED;
		art += "Your rank: Not ranked\n";
	}
	else {
		art += "Your rank: ";
		art += RED;
		for (size_t i = 0; i < rank.size(); i++) {
			if (rank[i].first == this->clients[fd]->getHighScore()) {
				art += std::to_string(i + 1);
				break;
			}
		}
		art += "\n";
	}
	art += RESET;
	return art;
}

std::string Server::botAttack(std::string &choice, int fd)
{
	srand(static_cast<unsigned int>(time(0)));

	int botChoiceNum = rand() % 3;
	std::string art;
	std::string arr[3] = {"ROCK", "SCISSORS", "PAPER"};
	art += "YOUR CHOICE: " + choice + "\n";
	art += "BOT'S CHOICE: " + arr[botChoiceNum] + "\n";

	int playerChoiceNum;
	for (playerChoiceNum = 0; playerChoiceNum < 3; playerChoiceNum++)
		if (arr[playerChoiceNum] == choice)
			break;
	if (playerChoiceNum == botChoiceNum)
		art += drawMsg(fd);
	else if ((playerChoiceNum + 1) % 3 == botChoiceNum) {
		this->clients[fd]->setHp(this->clients[fd]->getHp() - 1);
		art += loseMsg(fd);
		if (this->clients[fd]->getHp() == 0)
			art += botQuit(fd);
	}
	else {
		this->clients[fd]->setPlayingScore(this->clients[fd]->getPlayingScore() + 1);
		art += winMsg(fd);
	}
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
	YOU CAN ALSO USE "HELP" TO GET THIS MESSAGE.
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
	else if (cmd == "ROCK" || cmd == "PAPER" || cmd == "SCISSORS")
		return botAttack(cmd, fd);
	else
		return Response::failure(ERR_UNKNOWNCOMMAND, cmd, this->name, this->clients[fd]->getNickname());
	return "";
}
