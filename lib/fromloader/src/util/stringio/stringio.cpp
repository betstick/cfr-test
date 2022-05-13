#include "stringio.h"

namespace cfr
{
	std::string readString(UMEM* src)
	{
		std::string ret;
		bool nullFound = false;
		char c = 'a';

		while(nullFound == false)
		{
			uread(&c,sizeof(char),1,src);

			if(c == '\0')
				nullFound = true;
			else
				ret.push_back(c);
		}

		return ret;
	};

	std::wstring readWString(UMEM* src, bool endian)
	{
		std::wstring ret;
		bool nullFound = false;
		wchar_t c = 'a';

		while(nullFound == false)
		{
			ureade(&c,sizeof(wchar_t),1,src,endian);

			if(c == '\0')
				nullFound = true;
			else
				ret.push_back(c);
		}

		return ret;
	};

	std::string freadString(UMEM* src, bool nullTerm = true, int length = 0)
	{
		std::string ret = "";

		if(nullTerm)
		{
			bool found = false;

			while(!found)
			{
				char c = ugetc(src);

				if(ueod(src) != 0)
					throw std::runtime_error("Reached end of file while reading string...\n");
				if(c == 0)
					found = true;
				else
					ret.push_back(c);
			}

			return ret;
		}
		else
		{
			//this could be better optimized
			for(int i = 0; i < length; i++)
			{
				if(ueod(src) != 0)
					throw std::runtime_error("Reached end of file while reading string...\n");
				ret.push_back(ugetc(src));
			}

			return ret;
		}
	};

	std::pair<std::string,std::string> splitFullPath(std::string fullpath)
	{
		int len = fullpath.size();
		std::pair<std::string, std::string> combo;
		combo.first = "";
		combo.second = "";
		bool found = false;

		while(!found)
		{
			if(len < 1)
				throw std::runtime_error("Reached end of string while getting name...\n");

			char c = fullpath.at(len);

			if((c == '\\')||(c == '/'))	
				found = true;
			else
			{
				combo.first.insert(0,1,c);
				len--;
			}
		}
		
		while(len > 0)
		{
			char c = fullpath.at(len);
			combo.second.insert(0,1,c);
			len--;
		}
		
		return combo;
	};
};