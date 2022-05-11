#pragma once
#include "../binder.h"

namespace cfr
{
	class BDF3 : public Binder
	{
		private:
		class Header
		{

		};

		class BDF3_Entry
		{

		};

		Header header;
		std::vector<BDF3_Entry> internalEntries;

		public:
		//Init from path
		BDF3(const char* path);

		BDF3(UMEM* src);

		private:
		~BDF3();
	};
};