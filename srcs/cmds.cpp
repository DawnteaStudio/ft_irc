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

std::string Server::getFile(Request &request, int fd)
{
	if (request.args.size() < 2)
		return (Response::failure(ERR_NEEDMOREPARAMS, "GETFILE", this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	if (!this->clients[fd]->getIsRegistered())
		return (Response::failure(ERR_NOTREGISTERED, "", this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	if (this->channels.find(request.args[0]) == this->channels.end())
		return (Response::failure(ERR_NOSUCHCHANNEL, request.args[0], this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	if (this->channels[request.args[0]]->findFile(request.args[1]) == this->channels[request.args[0]]->getFiles().end())
		return (Response::failure(ERR_FILEERROR, request.args[1], this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	File file = this->channels[request.args[0]]->getFiles()[request.args[1]];
	std::fstream ofs(DOWNLOADED_FILE_PATH + request.args[1], std::fstream::out);
	if (ofs.fail())
		return (Response::failure(ERR_FILEERROR, request.args[1], this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	ofs << file.getFileContent();
	ofs.close();
	return "";
}

std::string Server::sendFile(Request &request, int fd)
{
	if (request.args.size() < 2)
		return (Response::failure(ERR_NEEDMOREPARAMS, "SENDFILE", this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	if (!this->clients[fd]->getIsRegistered())
		return (Response::failure(ERR_NOTREGISTERED, "", this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	std::string channelName = request.args[0];
	std::string filePath = request.args[1];

	if (this->channels.find(channelName) == this->channels.end())
		return (Response::failure(ERR_NOSUCHCHANNEL, channelName, this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	std::fstream ifs(request.args[1], std::fstream::in);
	if (ifs.fail())
		return (Response::failure(ERR_FILEERROR, request.args[1], this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	std::string fileName = request.args[1].substr(request.args[1].find_last_of('/') + 1);
	File file(fileName, channelName);
	if (this->channels[channelName]->findFile(fileName) != this->channels[channelName]->getFiles().end())
		return (Response::failure(ERR_FILEERROR, fileName, this->clients[fd]->getPrefix(), this->clients[fd]->getNickname()));
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	file.setFileContent(content);
	ifs.close();
	this->channels[channelName]->getFiles().insert(std::pair<std::string, File>(fileName, file));
	return "";
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

	std::vector<std::string> channelName;
	std::vector<std::string> keys;
	makeJoinVector(request, channelName, keys);
	for (size_t i = 0; i < channelName.size(); i++)
	{
		ErrorCode err = join(channelName[i], keys.size() > i ? keys[i] : "", fd);
		if (err != ERR_NONE)
			return Response::failure(err, channelName[i], this->name, this->clients[fd]->getNickname());
	}
	return "";
}

std::string Server::partChannel(Request &request, int fd)
{
	if (request.args.size() < 1)
		return Response::failure(ERR_NEEDMOREPARAMS, "PART", this->name, this->clients[fd]->getNickname());
	if (!this->clients[fd]->getIsRegistered())
		return Response::failure(ERR_NOTREGISTERED, "", this->name, this->clients[fd]->getNickname());
	
	std::vector<std::string> channelName;
	makePartVector(request, channelName);

	for (size_t i = 0; i < channelName.size(); i++)
	{
		ErrorCode err = part(channelName[i], fd);
		if (err != ERR_NONE)
			return Response::failure(err, channelName[i], this->name, this->clients[fd]->getNickname());
	}
	return "";
}