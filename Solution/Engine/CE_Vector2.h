#pragma once

#include <cmath>
#include <cassert>

template<typename T>
class CE_Vector2
{
public:
	CE_Vector2() { x = 0; y = 0; }
	CE_Vector2(T aValue) { x = aValue; y = aValue; }
	CE_Vector2(T aX, T aY) { x = aX; y = aY; }
	~CE_Vector2() {};

	CE_Vector2<T> operator-()
	{
		CE_Vector2<T> tmp = *this;
		tmp.x = -x;
		tmp.y = -y;
		return tmp;
	}

	T x;
	T y;
};

template <typename T>
T Length(const CE_Vector2<T>& aVector)
{
	return T(sqrt(Length2(aVector)));
}

template <typename T>
T Length2(const CE_Vector2<T>& aVector)
{
	return Dot(aVector, aVector);
}

template <typename T>
void Normalize(CE_Vector2<T>& aVector)
{
	aVector = GetNormalized(aVector);
}

template <typename T>
CE_Vector2<T> GetNormalized(const CE_Vector2<T>& aVector)
{
	T length = Length(aVector);
	if (length == 0)
	{
		return CE_Vector2<T>();
	}
	return CE_Vector2<T>(aVector / length);
}

template <typename T>
T Dot(const CE_Vector2<T>& aFirstVector, const CE_Vector2<T>& aSecondVector)
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