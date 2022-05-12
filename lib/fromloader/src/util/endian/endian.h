#include <stdio.h>
#include <stdexcept>
#include <cstring>
#include <stdlib.h> //needed for MSVC

namespace cfr
{
	//size_t getline2(char** lineptr, size_t* n, FILE* stream);

	uint16_t switchEndian(uint16_t i, bool bigEndian = true);
	int16_t  switchEndian( int16_t i, bool bigEndian = true);
	uint32_t switchEndian(uint32_t i, bool bigEndian = true);
	int32_t  switchEndian( int32_t i, bool bigEndian = true);
	uint64_t switchEndian(uint64_t i, bool bigEndian = true);
	int64_t  switchEndian( int64_t i, bool bigEndian = true);
};