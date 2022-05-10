#pragma once
#include "cmem.h"
#include "umem.h"
#include <stdio.h>
#include <string>
#include <vector>

#include "../util/util.h"

namespace cfr
{
	enum FILE_TYPE
	{
		FT_UNK,
		FT_BND3,
		FT_BND4,
		FT_FLVER0,
		FT_FLVER2
	};

	//an in memory blob of bytes
	class File
	{
		public:
		File* parent = nullptr;
		std::vector<File*> children = {};

		//bool bigEndian = false; //figure out later
		UMEM* data = nullptr;

		public:
		File(const char* path);
		File(UMEM* src);

		//returns type of file if possible
		FILE_TYPE getFileType();

		//closes file unless there's children, returns how many
		int close();

		//force closes file and all of its children, returns dead child count
		int forceClose();

		private:
		~File();
	};
};