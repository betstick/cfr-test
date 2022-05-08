#pragma once
#include "cmem.h"
#include <stdio.h>
#include <string>
#include <vector>

#include "../util/util.h"

namespace cfr
{
	//an on disk, or in binder, listing for a file
	class Entry
	{
		public:
		std::string name; //e.g. 'c5370.flver'
		std::string path; //eg 'FRPG:/DATA/CHR/C5370/'
		long offset; //null if in memory
		long uncompressedSize; //in bytes
		long compressedSize; //in bytes
		int id = 0;

		Entry(
			std::string name, std::string path, long offset, 
			long compressedSize, long uncompressedSize, int id
		);
	};

	//an in memory blob of bytes
	class File : public Entry
	{
		public:
		File* parent; //null for no parent
		std::vector<File*> children;
		bool bigEndian = false; //figure out later
		char** data;

		File(MEM* file);
		File(FILE* file);
		File(const char* path);
	};
};