#include "bnd3.h"

namespace cfr
{
	/*void BND3::operator=(File a)
	{
		BND3(a.data);
	};*/

	Entry::Entry(
		std::string name, std::string path, long offset, 
		long compressedSize, long uncompressedSize, long id
	)
	{
		this->name = name;
		this->path = path;
		this->offset = offset;
		this->compressedSize = compressedSize;
		this->uncompressedSize = uncompressedSize;
		this->id = id;
	};

	BND3::BND3_Entry::BND3_Entry(Header header, UMEM* src, long startPos)
	{
		uread(&this->rawFlags,sizeof(char),4,src);

		uread(&this->compressedSize,sizeof(int32_t),1,src);

		uread(&this->dataOffset,sizeof(uint32_t),1,src);

		//file id
		if(header.rawFormat & 0b01000000)
			uread(&this->id,sizeof(int32_t),1,src);

		//file name offset
		if((header.rawFormat & 0b00100000) | (header.rawFormat & 0b00010000))
			uread(&this->nameOffset,4,1,src);

		//file uncompressed size
		if(header.rawFormat & 0b00000100)
			uread(&this->uncompressedSize,sizeof(int32_t),1,src);
	};

	BND3::BND3(const char* path)
	{
		this->data = uopenFile(path,"rb");
		BND3(this->data);
	};

	BND3::BND3(File file)
	{
		this->data = file.data;
		BND3(this->data);
	};

	BND3::BND3(UMEM* src)
	{
		this->format = F_BND3;
		this->data = src;

		long startPos = utell(src);

		this->header = Header(src);

		uread(&this->header,sizeof(Header),1,src); 

		for(int32_t i = 0; i < this->header.fileCount; i++)
		{
			BND3_Entry* bndEntry = new BND3_Entry(this->header,src,startPos);
			this->internalEntries.push_back(*bndEntry);

			long position = utell(src);
			
			useek(src,bndEntry->nameOffset+startPos,SEEK_CUR);

			std::string fullpath = freadString(src);

			std::pair<std::string, std::string> pathName = splitFullPath(fullpath);

			useek(src,position,SEEK_SET);

			this->entries.push_back(Entry(
				pathName.first, pathName.second,
				bndEntry->dataOffset+startPos,
				bndEntry->compressedSize,
				bndEntry->uncompressedSize,
				bndEntry->id				
			));
		}

		//return to start, cleaned up
		useek(this->data,0,SEEK_SET);
	};
};