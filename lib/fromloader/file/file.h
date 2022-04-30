#pragma once
#include "../common.h"

namespace cfr
{
	class Handle
	{

	};
	class File
	{
		public:
		std::string name;
		long size;
		void* data;
		std::vector<File*> parents; //null means no parent
		int references;
		bool isCompressed; //virtual are in memory only
		bool compressor; //if children are compressed

		File();
		File(MEM* src);
		File(const char* path);


		~File();

		int open(); //0 on success, 1 on error
		int close(); //0 on success, otherwise number of files open
		std::string getType();
	};

	class Binder
	{
		std::vector<File*> children;
	};
};


//only dealloc decompressed files when they are all closed and the
//parent is also closed

//basically, don't dealloc 