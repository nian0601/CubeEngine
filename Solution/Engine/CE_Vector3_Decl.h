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
};
