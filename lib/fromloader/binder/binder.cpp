#include "binder.h"

namespace cfr
{
	File* Binder::loadFile(int index)
	{
		long totalSize = std::max(
			this->entries[index].compressedSize,
			this->entries[index].uncompressedSize
		);

		useek(this->data,this->entries[index].offset,SEEK_SET);

		char* fileData;
		uread(fileData,totalSize,1,this->data);

		//return to start to clean up
		useek(this->data,0,SEEK_SET);

		File* file = new File(uopenMem(fileData,totalSize));
		file->parent = this;

		return file;
	};

	File* Binder::loadFileName(std::string name)
	{
		//hash file name, then grab hash
		/*for(int i = 0; i < this->entries.size(); i++)
		{
			if(this->entries[i].name.compare(name) == 0)
			{
				return loadFile(i);
			}
		}*/

		return nullptr;
	};
};