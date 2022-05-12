#include "open.h"

//TODO: don't currently have a way record compression info
// nor is there a way to compress stuff. For now, compressed
// data cannot be written...

namespace cfr
{
	File* OpenFile(const char* path)
	{
		UMEM* src = uopenFile(path,"rb");
		return OpenFile(src);
	};

	File* OpenFile(UMEM* src)
	{
		FILE_FORMAT format = getFormat(src);

		UMEM* tmp = src;

		//decompress the DCX and substitue pointers
		if(format == FROM_DCX)
			tmp = openDCX(src);

		switch(format)
		{
			case FROM_DCX:
				throw std::runtime_error("Found double compression?\n");
			case FROM_BDF3:
				return new BDF3(tmp);
			case FROM_BDF4:
				return new BDF4(tmp);
			case FROM_BHD5_0:
				return new BHD5_0(tmp);
			case FROM_BHD5_1:
				return new BHD5_1(tmp);
			case FROM_BND3:
				return new BND3(tmp);
			case FROM_BND4:
				return new BND4(tmp);
			case UNKOWN_FORMAT:
				return nullptr;
		}
	};
}