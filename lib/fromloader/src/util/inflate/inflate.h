#pragma once
#include "../file/file.h"
#include <zlib.h>
#include <assert.h>

#ifndef ZLIB_INF__
#define ZLIB_INF__

//TODO: is this needed?
#ifdef _WIN32
#include "io.h"
#endif

#ifndef CHUNK
#	define CHUNK 16384
#endif

namespace cfr
{
	int inf(UMEM *source, UMEM *dest);
};

#endif