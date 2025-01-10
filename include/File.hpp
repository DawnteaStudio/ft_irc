#ifndef FILE_HPP
#define FILE_HPP

#include "Server.hpp"

class File {
public:
	File();
	~File();
	File(const File&);
	File &operator=(const File&);
};

#endif