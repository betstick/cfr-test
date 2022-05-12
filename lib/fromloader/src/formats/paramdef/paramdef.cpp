#include "paramdef.h"

namespace cfr
{
	PARAMDEF::i_Header::i_Header(UMEM* src)
	{
		//read ahead to get version and endianess
		long pos = utell(src);

		int16_t tempVer = 0;
		int8_t endian = 0;

		useek(src,0x2C,SEEK_CUR);
		uread(&endian,sizeof(int8_t),1,src);

		if(endian == -1)
			this->bigEndian = true;
		else
			this->bigEndian = false;

		useek(src,1,SEEK_CUR); //0x2E
		uread(&tempVer,sizeof(int16_t),1,src);

		//TODO: a macro would probably be a better solution...
		// if(bigEndian){var = switchEndian} //avoids extra ops
		// format macro function like switchEndian?
		tempVer = switchEndian(tempVer,endian);

#ifdef VALIDATE_ALL
		assertMsg(
			tempVer == 101 || tempVer == 102 ||
			tempVer == 103 || tempVer == 104 ||
			tempVer == 201 || tempVer == 202,
			"PARAMDEF::i_Header tempVer returned incorrect value!\n"
		);
#endif
		useek(src,pos,SEEK_SET);

		uread(&this->fileSize,sizeof(int32_t),1,src);
	};
};