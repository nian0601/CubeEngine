#pragma once

#include "CE_Vector3_Decl.h"
#include "CE_Vector4_Decl.h"

template<typename T>
class CE_Matrix44
{
public:
	CE_Matrix44();
	CE_Matrix44(const CE_Matrix44<T>& aMatrix);
	~CE_Matrix44();

	void Init(T* aDataPointer);
	T myMatrix[4 * 4];

	static CE_Matrix44<T> CreateRotateAroundX(T aAngleInRadians);
	static CE_Matrix44<T> CreateRotateAroundY(T aAngleInRadians);
	static CE_Matrix44<T> CreateRotateAroundZ(T aAngleInRadians);
	static CE_Matrix44<T> RotateAroundAxis(const CE_Vector3<T>& aAxis, T aAngleInRadians);
	static CE_Matrix44<T> CreateReflectionMatrixAboutAxis(const CE_Vector3<T>& aReflectionVector);
	static CE_Matrix44<T> CreateProjectionMatrixLH(T aNearZ, T aFarZ, T aAspectRatio, T aFovAngle);
	static CE_Matrix44<T> CreateOrthogonalMatrixLH(T aWidth, T aHeight, T aNearZ, T aFarZ);

	void SetPos(const CE_Vector3<T>& aPos);
	void SetPos(const CE_Vector4<T>& aPos);
	void SetPos(const T& aX, const T& aY, const T& aZ);
	void SetPos(const T& aX, const T& aY, const T& aZ, const T& aW);

	void SetForward(const CE_Vector3<T>& aForward);
	void SetUp(const CE_Vector3<T>& anUp);
	void SetRight(const CE_Vector3<T>& aRight);

	void NormalizeRotationVectors();

	CE_Vector3<T> GetPos() const;
	CE_Vector4<T> GetPos4() const;
	CE_Vector3<T> GetForward() const;
	CE_Vector3<T> GetUp() const;
	CE_Vector3<T> GetRight() const;


	inline CE_Matrix44<T> operator=(const CE_Matrix44<T>& aMatrix)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			myMatrix[i] = aMatrix.myMatrix[i];
		}
		return *this;
	}

private:
	void NormalizeVector(T* aMatrixEntry);
};