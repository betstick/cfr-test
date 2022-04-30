#include "file.h"

namespace cfr
{
	int File::open()
	{
		if(this->isOpen)
			return 1;
		else
		{
			this->isOpen = true;
		}
	};

	int File::close()
	{
		//check if the file has any open children
		if(this->references == 0)
		{
			if(this->compressor == true)
			{
				//hypothetically there should only be one
				//if the parent is a compressor
				while(this->children.size() > 0)
				{
					//dealloc and remove from the array
					this->children[0]->~File();
					this->children.erase(this->children.begin());
				}
			}

			return 0;
		}
		else
			return this->references;
	};
};