#pragma once
#include "cmem.h"
#include <stdio.h>
#include <string>
#include <vector>

//#include 
namespace cfr
{
	class BinderFile
	{
		public:
		std::string name;
		long size;
		void* data;
		int hash;
	};

	class Binder
	{
		public:
		std::vector<BinderFile> files;

		/* init from paths */
		Binder(const char* bnd);
		Binder(const char* bhd, const char* bdf);

		/* init from files */
		Binder(FILE* bnd);
		Binder(FILE* bhd, FILE* bdf);

		/* init from memory */
		Binder(MEM* bnd);
		Binder(MEM* bhd, MEM* bdf);

		private:
		long binderSize;

		std::string bndPath;
		std::string bhdPath;
		std::string bdfPath;
	};
}
