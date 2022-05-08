#include "bnd3.h"

namespace cfr
{
	//fill whole struct
	struct BndHeader
	{
		char magic[4];
		char version[8];
		uint8_t rawFormat;
		uint8_t bigEndian;
		uint8_t bitBigEndian;
		char unk0F;
		int32_t fileCount;
		int32_t fileHeadersEnd;
		int32_t unk18;
		int32_t unk1C; //assert 0
	};

	//fill whole struct
	struct BndFile
	{
		char rawFlags;
		char unk01;
		char unk02;
		char unk03;

		int32_t compressedSize;
		uint32_t dataOffset;

		//circumstationally fill these in
		//int32_t* id = NULL;
		//int32_t* nameOffset = NULL;
		//int32_t* uncompressedSize = NULL;
	};

	BND3::BND3(const char* path)
	{
		//forward to next constructor
		BND3(v_fopen(path, "rb"));
	};

	BND3::BND3(FILE* src)
	{
		this->format = F_BND3;
		this->isFile = true;
		this->file = src;

		long startPos = ftell(src);

		BndHeader* hdr = new BndHeader;
		fread(hdr,sizeof(BndHeader),1,src); 

		for(int32_t i = 0; i < hdr->fileCount; i++)
		{
			std::pair<std::string, std::string> pathName = {"",""};
			int32_t compressedSize = 0;
			int32_t uncompressedSize = 0;
			uint32_t dataOffset = 0;
			int32_t id = 0;

			fseek(src,sizeof(char) * 4,SEEK_CUR);
			fread(&compressedSize,sizeof(int32_t),1,src);
			fread(&dataOffset,sizeof(uint32_t),1,src);

			//file id
			if(hdr->rawFormat & 0b01000000)
			{
				fread(&id,sizeof(int32_t),1,src);
			}

			//file name offset
			if((hdr->rawFormat & 0b00100000) | (hdr->rawFormat & 0b00010000))
			{
				int32_t position = ftell(src);
				int32_t offset = 0;
				
				fread(&offset,4,1,src);
				fseek(src,offset+startPos,SEEK_CUR);

				std::string fullpath = freadString(src);
				pathName = splitFullPath(fullpath);

				fseek(src,position,SEEK_SET);
			}

			//file uncompressed size
			if(hdr->rawFormat & 0b00000100)
			{
				fread(&uncompressedSize,sizeof(int32_t),1,src);
			}

			this->files.push_back(Entry(
				pathName.first, pathName.second,
				dataOffset, 
				compressedSize, uncompressedSize,
				id
			));
		}
	};

	BND3::BND3(File* binder)
	{

	};
};