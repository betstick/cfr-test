#include "stringio.h"

namespace cfr
{
	std::string freadString(FILE* src, bool nullTerm = true, int length = 0)
	{
		std::string ret = "";

		if(nullTerm)
		{
			bool found = false;

			while(!found)
			{
				char c = fgetc(src);

				if(feof(src) != 0)
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
				if(feof(src) != 0)
					throw std::runtime_error("Reached end of file while reading string...\n");
				ret.push_back(fgetc(src));
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