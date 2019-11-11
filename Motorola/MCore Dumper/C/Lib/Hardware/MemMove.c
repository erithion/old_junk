#include "memmove.h"

typedef unsigned char  uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int   uint_t;

void memmove(void* dst, const void* src, unsigned int size)
{
	const uchar_t*  byteSrc = src;
	uchar_t*        byteDst = dst;
	const ushort_t* wordSrc = src;
	ushort_t*       wordDst = dst;
	uint_t          intSize = size / sizeof(ushort_t);
	uint_t          remainder = size % sizeof(ushort_t);
	uint_t          i = 0;
	
	while (i < intSize)
	{
		wordDst[i] = wordSrc[i];
		++i;
	}
	for (i = 0; i < remainder; ++i)
	{
		byteDst[i + intSize * sizeof(ushort_t)] = byteSrc[i + intSize * sizeof(ushort_t)];
	}
}
