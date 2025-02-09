#include <ctime>
#include "../include/Server.hpp"

void Server::updateHighScore(int fd)
{
	this->clients[fd]->setGameMode(false);
	if (this->clients[fd]->getPlayingScore() > this->clients[fd]->getHighScore())
		this->clients[fd]->setHighScore(this->clients[fd]->getPlayingScore());
}

std::string Server::drawMsg()
{
	std::string art = BLUE;
	art += " ____  ____      ___        ___ _ \n"
		"|  _ \\|  _ \\    / \\ \\      / / | |\n"
		"| | | | |_) |  / _ \\ \\ /\\ / /| | |\n"
		"| |_| |  _ <  / ___ \\ V  V / |_|_|\n"
		"|____/|_| \\_\\/_/   \\_\\_/\\_/  (_|_)\n";
	art += RESET;
	return art;
}

std::string Server::winMsg(int fd)
{
	std::string art = GREEN;
	art +=
		"__   __           __        ___       \n"
		"\\ \\ / /__  _   _  \\ \\      / (_)_ __  \n"
		" \\ V / _ \\| | | |  \\ \\ /\\ / /| | '_ \\ \n"
		"  | | (_) | |_| |   \\ V  V / | | | | |\n"
		"  |_|\\___/ \\__,_|    \\_/\\_/  |_|_| |_|\n";

	art += RESET;
	updateHighScore(fd);
	return art;
}

std::string Server::loseMsg(int fd)
{
	std::string art = RED;
	art +=
		"__   __            _                   \n"
		"\\ \\ / /__  _   _  | |    ___  ___  ___ \n"
		" \\ V / _ \\| | | | | |   / _ \\/ __|/ _ \\\n"
		"  | | (_) | |_| | | |__| (_) \\__ \\  __/\n"
		"  |_|\\___/ \\__,_| |_____\\___/|___/\\___|\n";

	art += RESET;
	updateHighScore(fd);
	return art;
}

std::string Server::botQuit(int fd)
{
	if (!this->clients[fd]->getGameMode())
		return Response::failure(ERR_NOTINGAME, "", this->name, this->clients[fd]->getNickname());

	std::string art =
		"  ____                 _   ____             \n"
		" / ___| ___   ___   __| | | __ ) _   _  ___ \n"
		"| |  _ / _ \\ / _ \\ / _` | |  _ \\| | | |/ _ \\\n"
		"| |_| | (_) | (_) | (_| | | |_) | |_| |  __/\n"
		" \\____|\\___/ \\___/ \\__,_| |____/ \\__, |\\___|\n"
		"                                 |___/      \n";

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

	std::string art =
		"__        __   _                          \n"
		"\\ \\      / /__| | ___ ___  _ __ ___   ___ \n"
		" \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n"
		"  \\ V  V /  __/ | (_| (_) | | | | | |  __/\n"
		"   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___| \n"
		"\n"
		"YOUR INPUT SHOULD BE IN THE FORM OF \"BOT <CHOICE>\".\n"
		"EXAMPLE: BOT ROCK\n"
		"\n"
		"    BOT ROCK          BOT SCISSORS           BOT PAPER\n"
		"    _______           _______                _______ \n"
		"---'   ____)      ---'   ____)____       ---'   ____)____\n"
		"      (_____)               ______)                ______)\n"
		"      (_____)            __________)               _______)\n"
		"      (____)            (____)                    _______)\n"
		"---.__(___)       ---.__(___)            ---.__________) \n";

	this->clients[fd]->setGameMode(true);
	this->clients[fd]->setPlayingScore(0);
	this->clients[fd]->setHp(5);

	return art;
}

std::string Server::botScore(int fd)
{
	std::string art = BLUE;

	std::ostringstream oss;
	oss << this->clients[fd]->getHighScore();
	art += "YOUR HIGH SCORE: " + oss.str() + "\n";

	if (this->clients[fd]->getGameMode()) {
		oss.str("");
		oss << this->clients[fd]->getPlayingScore();
		art += "YOUR CURRENT SCORE: " + oss.str() + "\n";

		oss.str("");
		oss << this->clients[fd]->getHp();
		art += "YOUR REMAINING HP: " + oss.str() + "\n";
	}
	art += RESET;
	return art;
}

std::string Server::botRank(int fd)
{
	std::string art = GOLD;
	art +=
		"  ______      _    _   _ _  _____ _   _  ______  \n"
		" / /  _ \\    / \\  | \\ | | |/ /_ _| \\ | |/ ___\\ \\ \n"
		"/ /| |_) |  / _ \\ |  \\| | ' / | ||  \\| | |  _ \\ \\\n"
		"\\ \\|  _ <  / ___ \\| |\\  | . \\ | || |\\  | |_| |/ /\n"
		" \\_\\_| \\_\\/_/   \\_\\_| \\_|_|\\_\\___|_| \\_|\\____/_/ \n";

	std::vector<std::pair<int, std::string> > rank;
	std::map<int, Client *>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); it++)
		rank.push_back(std::make_pair(it->second->getHighScore(), it->second->getNickname()));
	std::sort(rank.begin(), rank.end(), std::greater<std::pair<int, std::string> >());

	int sameRank = 1;
	std::ostringstream oss;
	for (size_t i = 0; i < rank.size(); i++)
	{
		if (rank[i].first == 0 || i == 5)
			break;

		if (i == 0) {
			oss << 1;
			art += oss.str() + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
			oss.str("");
		}
		else if (rank[i].first == rank[i - 1].first) {
			oss << sameRank;
			art += oss.str() + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
			oss.str("");
		}
		else {
			sameRank = i + 1;
			oss << (i + 1);
			art += oss.str() + ". " + rank[i].second + " : " + std::to_string(rank[i].first) + "\n";
			oss.str("");
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
				oss << (i + 1);
				art += oss.str();
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
		art += drawMsg();
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

std::string Server::botIntro()
{
	std::string guide = BLUE;

	guide += "\n\n\n\n";
	guide +=
		"\tYOUR OPPONENT IS A BOT.\n"
		"\tYOU CAN PLAY ROCK PAPER SCISSORS WITH THE BOT.\n"
		"\tYOU CAN CHOOSE BETWEEN ROCK, PAPER, SCISSORS.\n"
		"\tYOU CAN ALSO USE \"HELP\" TO GET THIS MESSAGE.\n"
		"\tYOUR INPUT SHOULD BE IN THE FORM OF \"BOT <CHOICE>\".\n"
		"\tEXAMPLE: BOT ROCK\n"
		"\n"
		"###########################################################\n"
		"\n"
		"    BOT ROCK          BOT SCISSORS           BOT PAPER\n"
		"    _______           _______                _______\n"
		"---'   ____)      ---'   ____)____       ---'   ____)____\n"
		"      (_____)               ______)                ______)\n"
		"      (_____)            __________)               _______)\n"
		"      (____)            (____)                    _______)\n"
		"---.__(___)       ---.__(___)            ---.__________)\n"
		"\n"
		"###########################################################\n"
		"\n"
		"\tYOU CAN ALSO USE THE FOLLOWING COMMANDS:\n"
		"\tVIEW GUIDE AGAIN: BOT HELP\n"
		"\tSTART THE GAME: BOT START\n"
		"\tQUIT THE GAME: BOT QUIT\n"
		"\tVIEW YOUR HIGH SCORE: BOT SCORE\n"
		"\tVIEW RANKING: BOT RANK\n"
		"\n";

	guide += RESET;
	return guide;
}

std::string Server::bot(Request &request, int fd)
{
	if (!this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_NOTREGISTERED, "", this->name, this->clients[fd]->getNickname());
	if (request.args.size() < 1)
		return botIntro();

	std::string cmd = request.args[0];
	makeUpper(cmd);
	if (cmd == "HELP")
		return botIntro();
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
