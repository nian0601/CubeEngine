#pragma once

template<typename T>
class CE_Matrix33
{
public:
	CE_Matrix33();
	CE_Matrix33(const CE_Matrix33<T>& aMatrix);
	~CE_Matrix33();

	T myMatrix[3 * 3];

	static CE_Matrix33<T> CreateRotateAroundX(T aAngleInRadians);
	static CE_Matrix33<T> CreateRotateAroundY(T aAngleInRadians);
	static CE_Matrix33<T> CreateRotateAroundZ(T aAngleInRadians);
	static CE_Matrix33<T> CreateReflectionMatrixAboutAxis(CE_Vector3<T> aReflectionVector);

	void Rotate2D(T aRadian);
	void SetRotation2D(T aRadian);
	void SetPos(const CE_Vector2<T>& aPos);
	void SetPos(const CE_Vector3<T>& aPos);
	CE_Vector2<T> GetPos() const;
	CE_Vector3<T> GetPos3() const;

	inline CE_Matrix33<T> operator=(const CE_Matrix33<T>& aMatrix)
	{
		for (unsigned char i = 0; i < 9; ++i)
		{
			myMatrix[i] = aMatrix.myMatrix[i];
		}
		return *this;
	}
};
