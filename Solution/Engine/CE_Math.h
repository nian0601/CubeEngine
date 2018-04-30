#pragma once
#include <stdlib.h>
#include "CE_Vector3.h"
#define CE_PI 3.14159265358979323846f

inline float CE_RandFloat()
{
	return static_cast<float>(rand() % 10000) / 10000.f;
}

inline float CE_RandClamped()
{
	return CE_RandFloat() - CE_RandFloat();
}

inline int CE_RandInt(int aMin, int aMax)
{
	return (rand() & (aMax - aMin + 1)) + aMin;
}

inline float CE_RoundToInt(float aFloat)
{
	int intCast = static_cast<int>(aFloat > 0.f ? aFloat + 0.5f : aFloat - 0.5f);
	return static_cast<float>(intCast);
}

inline void CE_RoundToInt(CE_Vector3f& aVector)
{
	aVector.x = CE_RoundToInt(aVector.x);
	aVector.y = CE_RoundToInt(aVector.y);
	aVector.z = CE_RoundToInt(aVector.z);
}

template <typename T>
inline T CE_Min(const T& aFirst, const T& aSecond)
{
	return aFirst < aSecond ? aFirst : aSecond;
}

template <typename T>
inline T CE_Max(const T& aFirst, const T& aSecond)
{
	return aFirst > aSecond ? aFirst : aSecond;
}

inline float CE_Lerp(float aStart, float aEnd, float aAlpha)
{
	return (aStart + (aEnd- aStart) * aAlpha);
}