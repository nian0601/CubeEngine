#pragma once

#include <cmath>
#include <cassert>
#include "CE_Vector2_Decl.h"

template <typename T>
T CE_Length(const CE_Vector2<T>& aVector)
{
	return T(sqrt(CE_Length2(aVector)));
}

template <typename T>
T CE_Length2(const CE_Vector2<T>& aVector)
{
	return CE_Dot(aVector, aVector);
}

template <typename T>
void CE_Normalize(CE_Vector2<T>& aVector)
{
	aVector = CE_GetNormalized(aVector);
}

template <typename T>
CE_Vector2<T> CE_GetNormalized(const CE_Vector2<T>& aVector)
{
	T length = CE_Length(aVector);
	if (length == 0)
	{
		return CE_Vector2<T>();
	}
	return CE_Vector2<T>(aVector / length);
}

template <typename T>
T CE_Dot(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	return aFirstVector.x * aSecondVector.x + aFirstVector.y * aSecondVector.y;
}

// operator with vector
template <typename T>
bool operator==(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	return aFirstVector.x == aSecondVector.x && aFirstVector.y == aSecondVector.y;
}

template <typename T>
bool operator!=(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	return !(aFirstVector == aSecondVector);
}

template <typename T>
CE_Vector2<T> operator+(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	CE_Vector2<T> result(aFirstVector);
	result += aSecondVector;
	return result;
}
template <typename T>
CE_Vector2<T> operator+=(CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	aFirstVector.x += aSecondVector.x;
	aFirstVector.y += aSecondVector.y;
	return aFirstVector;
}

template <typename T>
CE_Vector2<T> operator-(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	CE_Vector2<T> result(aFirstVector);
	result -= aSecondVector;
	return result;
}
template <typename T>
CE_Vector2<T> operator-=(CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	aFirstVector.x -= aSecondVector.x;
	aFirstVector.y -= aSecondVector.y;
	return aFirstVector;
}

template <typename T>
CE_Vector2<T> operator*(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	CE_Vector2<T> result(aFirstVector);
	result *= aSecondVector;
	return result;
}
template <typename T>
CE_Vector2<T> operator*=(CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	aFirstVector.x *= aSecondVector.x;
	aFirstVector.y *= aSecondVector.y;
	return aFirstVector;
}

template <typename T>
CE_Vector2<T> operator/(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	CE_Vector2<T> result(aFirstVector);
	result /= aSecondVector;
	return result;
}
template <typename T>
CE_Vector2<T> operator/=(CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
{
	assert(aSecondVector.x != 0 && aSecondVector.y != 0 && "Division by zero.");
	aFirstVector.x /= aSecondVector.x;
	aFirstVector.y /= aSecondVector.y;
	return aFirstVector;
}

// operator with scalar
template <typename T>
CE_Vector2<T> operator+(const CE_Vector2<T>& aVector, T aScalar)
{
	CE_Vector2<T> result(aVector);
	result += aScalar;
	return result;
}
template <typename T>
CE_Vector2<T> operator+(T aScalar, const CE_Vector2<T>& aVector)
{
	return aVector + aScalar;
}
template <typename T>
CE_Vector2<T> operator+=(CE_Vector2<T>& aVector, T aScalar)
{
	aVector.x += aScalar;
	aVector.y += aScalar;
	return aVector;
}

template <typename T>
CE_Vector2<T> operator-(const CE_Vector2<T>& aVector, T aScalar)
{
	CE_Vector2<T> result(aVector);
	result -= aScalar;
	return result;
}

template <typename T>
CE_Vector2<T> operator-=(CE_Vector2<T>& aVector, T aScalar)
{
	aVector.x -= aScalar;
	aVector.y -= aScalar;
	return aVector;
}

template <typename T>
CE_Vector2<T> operator*(const CE_Vector2<T>& aVector, T aScalar)
{
	CE_Vector2<T> result(aVector);
	result *= aScalar;
	return result;
}
template <typename T>
CE_Vector2<T> operator*(T aScalar, const CE_Vector2<T>& aVector)
{
	return aVector * aScalar;
}
template <typename T>
CE_Vector2<T> operator*=(CE_Vector2<T>& aVector, T aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	return aVector;
}

template <typename T>
CE_Vector2<T> operator/(const CE_Vector2<T>& aVector, T aScalar)
{
	CE_Vector2<T> result(aVector);
	result /= aScalar;
	return result;
}
template <typename T>
CE_Vector2<T> operator/=(CE_Vector2<T>& aVector, T aScalar)
{
	assert(aScalar != 0 && "Division by zero.");
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	return aVector;
}

template <typename T>
CE_Vector2<T> operator-(const CE_Vector2<T>& aVector)
{
	return CE_Vector2<T>(-aVector.x, -aVector.y);
}

//typedefs
typedef CE_Vector2<float> CE_Vector2f;
typedef CE_Vector2<int> CE_Vector2i;