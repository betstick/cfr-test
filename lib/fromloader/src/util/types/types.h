#pragma once

#include "umem.h"
#include <stdexcept>

namespace cfr
{
	enum FILE_FORMAT
	{
		UNKOWN_FORMAT,
		FROM_BDF3,
		FROM_BDF4,
		FROM_BHD5_0,
		FROM_BHD5_1,
		FROM_BHF3,
		FROM_BHF4,
		FROM_BND3,
		FROM_BND4,
		FROM_BTL,
		FROM_DCX,
		FROM_DRB,
		FROM_ENTRYFILELIST,
		FROM_ESD,
		FROM_EMEVD,
		FROM_FFX,
		FROM_FLVER0,
		FROM_FLVER2,
		FROM_FMG,
		FROM_FSB,
		FROM_FEV,
		FROM_ITL,
		FROM_GFX,
		FROM_GPARAM,
		FROM_LUA,
		FROM_MSB,
		FROM_MTD,
		FROM_NGP,
		FROM_NVA,
		FROM_NVM,
		FROM_PARAM,
		FROM_SL2,
		FROM_TAE,
		FROM_TPF,
		HAVOK_HKS,
		HAVOK_HKX,
		HAVOK_HKXPWV,
		MS_DDS,
	};

	//reads first few bytes of file to determine type
	FILE_FORMAT getFormat(UMEM* src);
};