#pragma once
#include "../stdafx.h"

namespace cfr
{
	//Returns a UMEM handle for a decompressed DCX file.
	UMEM* openDCX(const char* path);

	//Returns a UMEM handle for a decompressed DCX file.
	UMEM* openDCX(UMEM* src);
};