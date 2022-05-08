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

	class Binder : public File
	{
		public:
		std::vector<Entry> files = {};
		BinderFormat format = UNK;
		bool isFile = false;

		virtual File loadFileName(std::string name);

		virtual File loadFileHash(int hash);

		virtual File loadFileIndex(int entryIndex);

		virtual int write(const char* path);

		virtual int write(FILE* file);

		protected:
		FILE* file = nullptr;

		Binder(); //not to be used
	};
};