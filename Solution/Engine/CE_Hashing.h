#pragma once

#include "CE_Murmur.h"
#include "CE_String.h"

inline unsigned int Murmur(const CE_String& aValue)
{
	return aValue.GetHash();
}

inline unsigned int Murmur(int aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}

inline unsigned int Murmur(unsigned int aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}

inline unsigned int Murmur(unsigned long long aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}

inline unsigned int Murmur(long long aValue)
{
	unsigned int result = 0;
	MurmurHash3_x86_32(&aValue, 1, 2654435761, &result);

	return result;
}