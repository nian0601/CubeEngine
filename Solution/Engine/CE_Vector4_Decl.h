#pragma once

template<typename T>
class CE_Vector4
{
public:
	CE_Vector4() { x = 0; y = 0; z = 0; w = 0; }
	CE_Vector4(T aValue) { x = aValue; y = aValue; z = aValue; w = aValue; }
	CE_Vector4(T aX, T aY, T aZ, T aW) { x = aX; y = aY; z = aZ; w = aW; }
	~CE_Vector4() {}

	T x;
	T y;
	T z;
	T w;
};
