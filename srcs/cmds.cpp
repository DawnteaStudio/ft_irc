#include <algorithm>
#include <cctype>
#include "../include/Server.hpp"

std::string Server::setPassword(Request &request, int fd)
{
	if (request.args.size() < 1)
		return Response::failure(ERR_NEEDMOREPARAMS, "PASS", this->name, this->clients[fd]->getNickname());
	if (this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_ALREADYREGISTRED, "", this->name, this->clients[fd]->getNickname());
	if (this->password == request.args[0])
		this->clients[fd]->setIsValidPasswd(true);
	else
		this->clients[fd]->setIsValidPasswd(false);
	return "";
}

std::string Server::setUserNickname(Request &request, int fd)
{
	std::string nickname = this->clients[fd]->getNickname();

	if (request.args.size() < 1)
		return Response::failure(ERR_NONICKNAMEGIVEN, "", this->name, nickname);
	if (isSameNickname(request.args[0], nickname))
		return "";
	if (!isValidUserNickname(request.args[0]))
		return Response::failure(ERR_ERRONEUSNICKNAME, request.args[0], this->name, nickname);
	if (isUsedUserNickname(request.args[0]))
		return Response::failure(ERR_NICKNAMEINUSE, request.args[0], this->name, nickname);
	if (this->clients[fd]->getIsValidPasswd()) {
		if (nickname != "")
			deleteUserNickname(nickname);
		this->clients[fd]->setNickname(request.args[0]);
		addNewUserNickname(request.args[0]);
	}
	return "";
}

std::string Server::setUser(Request &request, int fd)
{
	if (request.args.size() < 4)
		return Response::failure(ERR_NEEDMOREPARAMS, "USER", this->name, this->clients[fd]->getNickname());
	if (this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_ALREADYREGISTRED, "", this->name, this->clients[fd]->getNickname());
	if (this->clients[fd]->getNickname() != "") {
		this->clients[fd]->setUserName(request.args[0]);
		this->clients[fd]->setRealName(request.args[3]);
		this->clients[fd]->setPrefix();
		this->clients[fd]->setIsRegistered(true);
	}
	return "";
}

std::string Server::setOper(Request &request, int i)
{
	if (request.args.size() != 2)
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Response::failure(ERR_NEEDMOREPARAMS, "OPER", this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
	if (request.args[1] != this->password)
		return (createMessage(ERR_PASSWDMISMATCH, this->clients[i]->getNickname(), Response::failure(ERR_PASSWDMISMATCH, "", this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
	this->clients[i]->setIsOperator(true);
	return (createMessage(RPL_YOUREOPER, this->clients[i]->getNickname(), Response::success(RPL_YOUREOPER, "", this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
}

std::string Server::getFile(Request &request, int i)
{
	if (request.args.size() != 2)
		return (createMessage(ERR_NEEDMOREPARAMS, this->clients[i]->getNickname(), Response::failure(ERR_NEEDMOREPARAMS, "GETFILE", this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
	if (!this->clients[i]->getIsRegistered())
		return (createMessage(ERR_NOTREGISTERED, this->clients[i]->getNickname(), Response::failure(ERR_NOTREGISTERED, "", this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
	if (!isUsedUserNickname(request.args[0]))
		return (createMessage(ERR_NOSUCHNICK, this->clients[i]->getNickname(), Response::failure(ERR_NOSUCHNICK, request.args[0], this->clients[i]->getPrefix(), this->clients[i]->getNickname())));
	
	
}

void Server::quit(int fd)
{
	// std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	// Client *client = this->clients[fd];
	// if (client->getChannels().size() > 0)
	// {
	// 	for (std::vector<Channel *>::iterator it = client->getChannels().begin(); it != client->getChannels().end(); it++)
	// 		(*it)->quit(client);
	// }
	this->removeClient(fd);
	close(fd);
}

std::string Server::joinChannel(Request &request, int fd)
{
	if (request.args.size() < 1)
		return Response::failure(ERR_NEEDMOREPARAMS, "JOIN", this->name, this->clients[fd]->getNickname());
	if (!this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_NOTREGISTERED, "", this->name, this->clients[fd]->getNickname());

	std::vector<std::string> channels;
	std::vector<std::string> keys;
	makeJoinVector(request, channels, keys);
	for (size_t i = 0; i < channels.size(); i++)
	{
		ErrorCode err = join(channels[i], keys.size() > i ? keys[i] : "", fd, false);
		if (err != ERR_NONE)
			return Response::failure(err, channels[i], this->name, this->clients[fd]->getNickname());
	}
	
	if (request.args[0][0] != '#' && request.args[0][0] != '&')
		return Response::failure(ERR_NOSUCHCHANNEL, request.args[0], this->name, this->clients[fd]->getNickname());
	// if (request.args.size() == 1)
	// 	return this->joinChannel(request.args[0], "", fd);
	// else
	// 	return this->joinChannel(request.args[0], request.args[1], fd);
	return "";
}