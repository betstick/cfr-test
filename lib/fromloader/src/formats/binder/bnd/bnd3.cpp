#include "bnd3.h"

namespace cfr
{
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

	/*Entry* BND3::BND3_Entry::newEntry(UMEM* src, int startPos)
	{
		long position = utell(src);
			
		useek(src,this->nameOffset+startPos,SEEK_CUR);

		std::string fullpath = freadString(src);

		std::pair<std::string, std::string> pathName = splitFullPath(fullpath);

		useek(src,position,SEEK_SET);

		return new Entry(
			pathName.first, pathName.second,
			this->dataOffset+startPos,
			this->compressedSize,
			this->uncompressedSize,
			this->id				
		);
	};*/

	BND3::Header::Header()
	{
		//do nothing
	};

	BND3::Header::Header(UMEM* src)
	{
		uread(this,sizeof(Header),1,src);
	};

	BND3::BND3(const char* path) : Binder(path)
	{
		this->data = uopenFile(path,"rb");
		BND3(this->data);
	};

	BND3::BND3(UMEM* src) : Binder(src)
	{
		this->format = FROM_BND3;
		this->data = src;

		long startPos = utell(src);

		//confirm its not a DCX
		char magic[4];
		uread(magic,4,1,src);
		useek(src,startPos,SEEK_SET);

		if(strncmp(magic,"DCX\0",4) == 0)
			this->data = openDCX(src);

		this->header = Header(src);

		//uread(&this->header,sizeof(Header),1,src); 

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