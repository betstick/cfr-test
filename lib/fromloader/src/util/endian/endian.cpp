#include "endian.h"

namespace cfr
{
	/*unsigned int reverseU32(unsigned int x)
	{
		x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
		x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
		x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
		x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
		return((x >> 16) | (x << 16));
	}

	unsigned int bitswap(unsigned int i)
	{
#ifdef _GNU
		return __builtin_bswap32(i);
#else
		return reverseU32(i);
#endif
	};*/

	uint16_t switchEndian(uint16_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap16(i);
#elif defined(_MSC_VER)
			return _byteswap_ushort(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	int16_t switchEndian(int16_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap16(i);
#elif defined(_MSC_VER)
			return _byteswap_ushort(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	uint32_t switchEndian(uint32_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap32(i);
#elif defined(_MSC_VER)
			return _byteswap_ulong(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	int32_t switchEndian(int32_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap32(i);
#elif defined(_MSC_VER)
			return _byteswap_ulong(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	uint64_t switchEndian(uint64_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap64(i);
#elif defined(_MSC_VER)
			return _byteswap_uint64(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	int64_t switchEndian(int64_t i, bool bigEndian = true)
	{
		if(bigEndian)
#ifdef _GNU
			return __builtin_bswap64(i);
#elif defined(_MSC_VER)
			return _byteswap_uint64(i);
#else
			throw std::runtime_error("Undefined byteswap!");
#endif
		else
			return i;
	};

	/*void switchEndian(void* target, size_t size)
	{
		switch(size)
		{
			case(16):
			{
				uint16_t i = __builtin_bswap16(*(uint16_t*)target);
				memcpy(target,&i,sizeof(uint16_t));
			}
			case(32):
			{
				uint32_t i = __builtin_bswap32(*(uint32_t*)target);
				memcpy(target,&i,sizeof(uint32_t));
			}
			case(64):
			{
				uint64_t i = __builtin_bswap64(*(uint64_t*)target);
				memcpy(target,&i,sizeof(uint64_t));
			}
			case(128):
			{
				__uint128_t i = __builtin_bswap128(*(__uint128_t*)target);
				memcpy(target,&i,sizeof(__uint128_t));
			}
			default: throw std::runtime_error("Invalid bytecount!\n");
		}
	};*/

/*	size_t getline2(char** lineptr, size_t* n, FILE* stream)
	{
#ifdef _GNU
		return getline(lineptr,n,stream);
#else
		int i = 0;

		while(feof(stream))
		{

		}
#endif
	};*/
};