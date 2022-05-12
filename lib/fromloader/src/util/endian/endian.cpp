#include "endian.h"

namespace cfr
{
	uint16_t switchEndian(uint16_t i, bool bigEndian = true)
	{
		if(bigEndian)
#if defined(__GNUC__) || defined(__clang__)
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
#if defined(__GNUC__) || defined(__clang__)
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
#if defined(__GNUC__) || defined(__clang__)
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
#if defined(__GNUC__) || defined(__clang__)
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
#if defined(__GNUC__) || defined(__clang__)
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
#if defined(__GNUC__) || defined(__clang__)
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