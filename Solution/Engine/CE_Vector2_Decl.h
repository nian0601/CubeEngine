#pragma once
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

