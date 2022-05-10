#include "file.h"

namespace cfr
{
	File::File(const char* path)
	{
		this->data = uopenFile(path,"rb");
	};

	File::File(UMEM* src)
	{
		this->data = src;
	};


	int File::close()
	{
		if(this->children.size() > 0)
			return this->children.size();
		else
			delete this;
			return 0;
	};

	int File::forceClose()
	{
		int c = this->children.size();
		int k = 0;

		if(c > 0)
		{
			while(c > 0)
			{
				if(this->children[c] != nullptr)
					k += this->children[c]->forceClose();
			}

			return k;
		}
		else
		{
			delete this;
			return 0;
		}
	};
};