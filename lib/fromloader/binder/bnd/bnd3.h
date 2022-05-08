#pragma once
#include "../binder.h"

namespace cfr
{
	class BND3 : public Binder
	{
		public:
		//init via file class
		BND3(File* binder);

		//init via mem handle
		//BND3(MEM* binder);

		//init via file handle
		BND3(FILE* binder);

		//init via path
		BND3(const char* path);

		File loadFileName(std::string name);

		File loadFileHash(int hash);

		File loadFileIndex(int entryIndex);

		int write(const char* path);

		int write(FILE* file);

		private:
		void index();
		void init();
	};
};