#pragma once
#include "formats/include.h"

#ifndef __OPEN_FILE
#define __OPEN_FILE

namespace cfr
{
	//Open file from disk. Null if fails.
	File* OpenFile(const char* path);

	//Open file from memory. Null if fails.
	File* OpenFile(UMEM* src);
};

#endif