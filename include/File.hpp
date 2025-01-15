#ifndef FILE_HPP
#define FILE_HPP

#include "Server.hpp"

class File {
public:
	File();
	File(std::string&, std::string&);
	~File();
	File(const File&);
	File &operator=(const File&);

	std::string filePath;
	std::string channelName;
	std::string fileContent;
	void setFileContent(std::string &);
	std::string getFileContent();
};

#endif