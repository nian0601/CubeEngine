#pragma once

template<typename T>
class CE_Vector3
{
public:
	CE_Vector3() { x = 0; y = 0; z = 0; }
	CE_Vector3(T aValue) { x = aValue; y = aValue; z = aValue; }
	CE_Vector3(T aX, T aY, T aZ) { x = aX; y = aY; z = aZ; }
	~CE_Vector3() {}

	T x;
	T y;
	T z;
	
	T& operator[](int aIndex)
	{
		CE_ASSERT(aIndex >= 0 && aIndex < 3, "Invalid index");
		if (aIndex == 0)
			return x;

		if (aIndex == 1)
			return y;

		return z;
	}

	const T& operator[](int aIndex) const
	{
		CE_ASSERT(aIndex >= 0 && aIndex < 3, "Invalid index");
		if (aIndex == 0)
			return x;

		if (aIndex == 1)
			return y;

		return z;
	}
};
