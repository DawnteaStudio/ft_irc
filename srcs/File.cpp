#include "../include/File.hpp"

File::File() {}

File::~File() {}

File::File(const File &other)
{
	*this = other;
}

File &File::operator=(const File &other)
{
	if (this != &other)
	{
	}
	return *this;
}
