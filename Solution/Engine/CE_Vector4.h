#pragma once

#include <cmath>
#include <cassert>
#include "CE_Vector4_Decl.h"

template <typename T>
T CE_Length(const CE_Vector4<T>& aVector)
{
	return sqrt(CE_Length2(aVector));
}

template <typename T>
T CE_Length2(const CE_Vector4<T>& aVector)
{
	return CE_Dot(aVector, aVector);
}

template <typename T>
void CE_Normalize(CE_Vector4<T>& aVector)
{
	if (aVector.x == 0 && aVector.y == 0 && aVector.z == 0 && aVector.w == 0)
	{
		//DL_DEBUG("Warning: Normalize zero vector.");
		return;
	}
	aVector = CE_GetNormalized(aVector);
}

template <typename T>
CE_Vector4<T> CE_GetNormalized(const CE_Vector4<T>& aVector)
{
	T length = CE_Length(aVector);
	if (length == 0)
	{
		return aVector;
	}
	//assert(length != 0 && "Division by zero.");
	return CE_Vector4<T>(aVector / length);
}

template <typename T>
T CE_Dot(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	return aFirstVector.x * aSecondVector.x + aFirstVector.y * aSecondVector.y + aFirstVector.z * aSecondVector.z + aFirstVector.w * aSecondVector.w;
}

// operator with vector

template <typename T>
bool operator==(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	return aFirstVector.x == aSecondVector.x && aFirstVector.y == aSecondVector.y && aFirstVector.z == aSecondVector.z && aFirstVector.w == aSecondVector.w;
}

template <typename T>
bool operator!=(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	return !(aFirstVector == aSecondVector);
}

template <typename T>
CE_Vector4<T> operator+(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	CE_Vector4<T> result(aFirstVector);
	result += aSecondVector;
	return result;
}
template <typename T>
CE_Vector4<T> operator+=(CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	aFirstVector.x += aSecondVector.x;
	aFirstVector.y += aSecondVector.y;
	aFirstVector.z += aSecondVector.z;
	aFirstVector.w += aSecondVector.w;
	return aFirstVector;
}

template <typename T>
CE_Vector4<T> operator-(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	CE_Vector4<T> result(aFirstVector);
	result -= aSecondVector;
	return result;
}
template <typename T>
CE_Vector4<T> operator-=(CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	aFirstVector.x -= aSecondVector.x;
	aFirstVector.y -= aSecondVector.y;
	aFirstVector.z -= aSecondVector.z;
	aFirstVector.w -= aSecondVector.w;
	return aFirstVector;
}

template <typename T>
CE_Vector4<T> operator*(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	CE_Vector4<T> result(aFirstVector);
	result *= aSecondVector;
	return result;
}
template <typename T>
CE_Vector4<T> operator*=(CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	aFirstVector.x *= aSecondVector.x;
	aFirstVector.y *= aSecondVector.y;
	aFirstVector.z *= aSecondVector.z;
	aFirstVector.w *= aSecondVector.w;
	return aFirstVector;
}

template <typename T>
CE_Vector4<T> operator/(const CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	CE_Vector4<T> result(aFirstVector);
	result /= aSecondVector;
	return result;
}
template <typename T>
CE_Vector4<T> operator/=(CE_Vector4<T>& aFirstVector, const CE_Vector4<T>& aSecondVector)
{
	assert(aSecondVector.x != 0 && aSecondVector.y != 0 && aSecondVector.z != 0 && aSecondVector.w != 0 && "Division by zero.");
	aFirstVector.x /= aSecondVector.x;
	aFirstVector.y /= aSecondVector.y;
	aFirstVector.z /= aSecondVector.z;
	aFirstVector.w /= aSecondVector.w;
	return aFirstVector;
}

// operator with scalar

template <typename T>
CE_Vector4<T> operator+(const CE_Vector4<T>& aVector, T aScalar)
{
	CE_Vector4<T> result(aVector);
	result += aScalar;
	return result;
}
template <typename T>
CE_Vector4<T> operator+(T aScalar, const CE_Vector4<T>& aVector)
{
	return aVector + aScalar;
}
template <typename T>
CE_Vector4<T> operator+=(CE_Vector4<T>& aVector, T aScalar)
{
	aVector.x += aScalar;
	aVector.y += aScalar;
	aVector.z += aScalar;
	aVector.w += aScalar;
	return aVector;
}

template <typename T>
CE_Vector4<T> operator-(const CE_Vector4<T>& aVector, T aScalar)
{
	CE_Vector4<T> result(aVector);
	result -= aScalar;
	return result;
}
template <typename T>
CE_Vector4<T> operator-=(CE_Vector4<T>& aVector, T aScalar)
{
	aVector.x -= aScalar;
	aVector.y -= aScalar;
	aVector.z -= aScalar;
	aVector.w -= aScalar;
	return aVector;
}

template <typename T>
CE_Vector4<T> operator*(const CE_Vector4<T>& aVector, T aScalar)
{
	CE_Vector4<T> result(aVector);
	result *= aScalar;
	return result;
}
template <typename T>
CE_Vector4<T> operator*(T aScalar, const CE_Vector4<T>& aVector)
{
	return aVector * aScalar;
}
template <typename T>
CE_Vector4<T> operator*=(CE_Vector4<T>& aVector, T aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	aVector.z *= aScalar;
	aVector.w *= aScalar;
	return aVector;
}

template <typename T>
CE_Vector4<T> operator/(const CE_Vector4<T>& aVector, T aScalar)
{
	CE_Vector4<T> result(aVector);
	result /= aScalar;
	return result;
}
template <typename T>
CE_Vector4<T> operator/=(CE_Vector4<T>& aVector, T aScalar)
{
	//assert(aScalar != 0 && "Division by zero.");
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	aVector.z /= aScalar;
	aVector.w /= aScalar;
	return aVector;
}

template <typename T>
CE_Vector4<T> operator-(const CE_Vector4<T>& aVector)
{
	return CE_Vector4<T>(-aVector.x, -aVector.y, -aVector.z, -aVector.w);
}

//typedefs
typedef CE_Vector4<float> CE_Vector4f;
typedef CE_Vector4<int> CE_Vector4i;