#include "open.h"

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

		switch(format)
		{
			case FROM_BDF3:
				return new BDF3(src);
			case FROM_BDF4:
				return new BDF4(src);
			case FROM_BHD5_0:
				return new BHD5_0(src);
			case FROM_BHD5_1:
				return new BHD5_1(src);
			case FROM_BND3:
				return new BND3(src);
			case FROM_BND4:
				return new BND4(src);
			case UNKOWN_FORMAT:
				return nullptr;
		}
	};
}