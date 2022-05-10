#pragma once
#include "../file/file.h"

namespace cfr
{
	enum BinderFormat 
	{
		F_UNK = 0,
		F_BND3,
		F_BND4,
		F_BHD5_0,
		F_BHD5_1,
		F_BHDP3, //binder header data pair (bhf3+bdf3)
		F_BHDP4, // ^^^^^^ version 4
	};

	//listing for a file in a bnd, not all members exist in each binder
	class Entry
	{
		public:
		std::string name = ""; //e.g. 'c5370.flver'
		std::string path = ""; //eg 'FRPG:/DATA/CHR/C5370/'
		long hash = 0;
		long offset = 0; //relative to start of binder data
		long uncompressedSize = 0; //in bytes
		long compressedSize   = 0; //in bytes
		long id = 0;

		Entry();

		Entry(
			std::string name, std::string path, long offset, 
			long compressedSize, long uncompressedSize, long id
		);
	};

	class Binder : public File
	{
		public:
		BinderFormat format = UNK;
		std::vector<Entry> entries = {};

		File* loadFile(int index);

		File* loadFileId(int entryID);

		File* loadFileHash(int hash);

		File* loadFileName(std::string name);

		//virtual int write(const char* path);

		//virtual int write(FILE* file);

		protected:
		Binder();
		~Binder();

		//Binder(File file);
	};
};