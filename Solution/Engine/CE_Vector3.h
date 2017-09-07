#pragma once

#include <cmath>
#include <cassert>
#include "CE_Vector3_Decl.h"

template <typename T>
T Length(const CE_Vector3<T>& aVector)
{
	return sqrt(Length2(aVector));
}

template <typename T>
T Length2(const CE_Vector3<T>& aVector)
{
	return Dot(aVector, aVector);
}

template <typename T>
void Normalize(CE_Vector3<T>& aVector)
{
	if (aVector.x == 0 && aVector.y == 0 && aVector.z == 0)
	{
		//DL_DEBUG("Warning: Normalize zero vector.");
		return;
	}
	aVector = GetNormalized(aVector);
}

template <typename T>
CE_Vector3<T> GetNormalized(const CE_Vector3<T>& aVector)
{
	T length = Length(aVector);
	if (length == 0)
	{
		return aVector;
	}
	//assert(length != 0 && "Division by zero.");
	return CE_Vector3<T>(aVector / length);
}

template <typename T>
T Dot(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	return aFirstVector.x * aSecondVector.x + aFirstVector.y * aSecondVector.y + aFirstVector.z * aSecondVector.z;
}

template <typename T>
CE_Vector3<T> Cross(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	return CE_Vector3<T>(
		aFirstVector.y * aSecondVector.z - aFirstVector.z * aSecondVector.y,
		aFirstVector.z * aSecondVector.x - aFirstVector.x * aSecondVector.z,
		aFirstVector.x * aSecondVector.y - aFirstVector.y * aSecondVector.x);
}

// operator with vector

template <typename T>
bool operator==(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	return aFirstVector.x == aSecondVector.x && aFirstVector.y == aSecondVector.y && aFirstVector.z == aSecondVector.z;
}

template <typename T>
bool operator!=(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	return !(aFirstVector == aSecondVector);
}

template <typename T>
CE_Vector3<T> operator+(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	CE_Vector3<T> result(aFirstVector);
	result += aSecondVector;
	return result;
}
template <typename T>
CE_Vector3<T> operator+=(CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	aFirstVector.x += aSecondVector.x;
	aFirstVector.y += aSecondVector.y;
	aFirstVector.z += aSecondVector.z;
	return aFirstVector;
}

template <typename T>
CE_Vector3<T> operator-(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	CE_Vector3<T> result(aFirstVector);
	result -= aSecondVector;
	return result;
}
template <typename T>
CE_Vector3<T> operator-=(CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	aFirstVector.x -= aSecondVector.x;
	aFirstVector.y -= aSecondVector.y;
	aFirstVector.z -= aSecondVector.z;
	return aFirstVector;
}

template <typename T>
CE_Vector3<T> operator*(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	CE_Vector3<T> result(aFirstVector);
	result *= aSecondVector;
	return result;
}
template <typename T>
CE_Vector3<T> operator*=(CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	aFirstVector.x *= aSecondVector.x;
	aFirstVector.y *= aSecondVector.y;
	aFirstVector.z *= aSecondVector.z;
	return aFirstVector;
}

template <typename T>
CE_Vector3<T> operator/(const CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	CE_Vector3<T> result(aFirstVector);
	result /= aSecondVector;
	return result;
}
template <typename T>
CE_Vector3<T> operator/=(CE_Vector3<T>& aFirstVector, const CE_Vector3<T>& aSecondVector)
{
	assert(aSecondVector.x != 0 && aSecondVector.y != 0 && aSecondVector.z != 0 && "Division by zero.");
	aFirstVector.x /= aSecondVector.x;
	aFirstVector.y /= aSecondVector.y;
	aFirstVector.z /= aSecondVector.z;
	return aFirstVector;
}

// operator with scalar

template <typename T>
CE_Vector3<T> operator+(const CE_Vector3<T>& aVector, T aScalar)
{
	CE_Vector3<T> result(aVector);
	result += aScalar;
	return result;
}
template <typename T>
CE_Vector3<T> operator+(T aScalar, const CE_Vector3<T>& aVector)
{
	return aVector + aScalar;
}
template <typename T>
CE_Vector3<T> operator+=(CE_Vector3<T>& aVector, T aScalar)
{
	aVector.x += aScalar;
	aVector.y += aScalar;
	aVector.z += aScalar;
	return aVector;
}

template <typename T>
CE_Vector3<T> operator-(const CE_Vector3<T>& aVector, T aScalar)
{
	CE_Vector3<T> result(aVector);
	result -= aScalar;
	return result;
}
template <typename T>
CE_Vector3<T> operator-=(CE_Vector3<T>& aVector, T aScalar)
{
	aVector.x -= aScalar;
	aVector.y -= aScalar;
	aVector.z -= aScalar;
	return aVector;
}

template <typename T>
CE_Vector3<T> operator*(const CE_Vector3<T>& aVector, T aScalar)
{
	CE_Vector3<T> result(aVector);
	result *= aScalar;
	return result;
}
template <typename T>
CE_Vector3<T> operator*(T aScalar, const CE_Vector3<T>& aVector)
{
	return aVector * aScalar;
}
template <typename T>
CE_Vector3<T> operator*=(CE_Vector3<T>& aVector, T aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	aVector.z *= aScalar;
	return aVector;
}

template <typename T>
CE_Vector3<T> operator/(const CE_Vector3<T>& aVector, T aScalar)
{
	CE_Vector3<T> result(aVector);
	result /= aScalar;
	return result;
}
template <typename T>
CE_Vector3<T> operator/=(CE_Vector3<T>& aVector, T aScalar)
{
	//assert(aScalar != 0 && "Division by zero.");
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	aVector.z /= aScalar;
	return aVector;
}

template <typename T>
CE_Vector3<T> operator-(const CE_Vector3<T>& aVector)
{
	return CE_Vector3<T>(-aVector.x, -aVector.y, -aVector.z);
}

template <typename T>
CE_Vector3<T> Reflect(const CE_Vector3<T>& aDirection, const CE_Vector3<T>& aNormal)
{
	return (aDirection - 2.f * CU::Dot(aDirection, aNormal) * aNormal);
}

//typedefs
typedef CE_Vector3<float> CE_Vector3f;
typedef CE_Vector3<int> CE_Vector3i;