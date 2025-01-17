#include "../include/File.hpp"

File::File() {}

File::~File() {}

File::File(std::string &name, std::string &channel)
{
	this->fileName = name;
	this->channelName = channel;
}

File::File(const File &other)
{
	*this = other;
}

File &File::operator=(const File &other)
{
	if (this != &other)
	{
		this->fileName = other.fileName;
		this->channelName = other.channelName;
	}
	return *this;
}
 