#pragma once
#include <stdio.h>
#include <string>
#include "umem.h"
#include <stdexcept>
#include <stdbool.h>

namespace cfr
{
	//reads string till length or null term and returns it
	//throws exceptions on eof
	std::string freadString(UMEM* src, bool nullTerm = true, int length = 0);

	//returns just the file name when given a full path
	//throws exceptions if it doesn't get what it wants
	//std::string fileNameFromPath(std::string path);

	//returns the file name and the path as a pair
	std::pair<std::string,std::string> splitFullPath(std::string fullpath);
};