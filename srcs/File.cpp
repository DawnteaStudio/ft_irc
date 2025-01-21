#include "../include/File.hpp"

File::File() {}

File::~File() {}

File::File(std::string &path, std::string &channel)
{
	this->filePath = path;
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
		this->filePath = other.filePath;
		this->channelName = other.channelName;
		this->fileContent = other.fileContent;
	}
	return *this;
}

void File::setFileContent(std::string &content) { this->fileContent = content; }

std::string File::getFileContent() { return this->fileContent; }
