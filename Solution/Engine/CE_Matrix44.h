#pragma once

#include "CE_Vector3.h"
#include "CE_Vector4.h"

class CE_Matrix44f
{
public:
	CE_Matrix44f();
	CE_Matrix44f(const CE_Matrix44f& aMatrix);
	~CE_Matrix44f(){}

	union
	{
		float myMatrix[4 * 4];
		struct  
		{
			CE_Vector4f myRight;
			CE_Vector4f myUp;
			CE_Vector4f myForward;
			CE_Vector4f myPosition;
		} myVec4s;
		struct
		{
			CE_Vector3f myRight;
			float __filler1;
			CE_Vector3f myUp;
			float __filler2;
			CE_Vector3f myForward;
			float __filler3;
			CE_Vector3f myPosition;
			float __filler4;
		} myVec3s;
		CE_Vector4f myRows[4];

		struct vec3Row
		{
			CE_Vector3f myRow;
			float __filler;
		};
		vec3Row myRow3s[4];
	};

	static CE_Matrix44f CreateRotateAroundX(float aAngleInRadians);
	static CE_Matrix44f CreateRotateAroundY(float aAngleInRadians);
	static CE_Matrix44f CreateRotateAroundZ(float aAngleInRadians);
	static CE_Matrix44f CreateReflectionMatrixAboutAxis(const CE_Vector3f& aReflectionVector);
	static CE_Matrix44f CreateProjectionMatrixLH(float  aNearZ, float  aFarZ, float aAspectRatio, float aFovAngle);
	static CE_Matrix44f CreateOrthogonalMatrixLH(float  aWidth, float aHeight, float  aNearZ, float  aFarZ);

	
	void SetPos(const CE_Vector3f& aPos) { myVec3s.myPosition = aPos; }
	void SetPos(const CE_Vector4f& aPos) { myVec4s.myPosition = aPos; }
	void SetPos(float aX, float aY, float aZ) { myVec3s.myPosition.x = aX; myVec3s.myPosition.y = aY; myVec3s.myPosition.z = aZ;}
	void SetPos(float aX, float aY, float aZ, float aW) {	myVec4s.myPosition.x = aX; myVec4s.myPosition.y = aY; myVec4s.myPosition.z = aZ; myVec4s.myPosition.w = aW;}

	void SetRight(const CE_Vector3f& aRight) { myVec3s.myRight = aRight; }
	void SetUp(const CE_Vector3f& aUp) { myVec3s.myUp = aUp; }
	void SetForward(const CE_Vector3f& aForward) { myVec3s.myForward= aForward; }

	const CE_Vector3f& GetPos() const { return myVec3s.myPosition; }
	const CE_Vector4f& GetPos4() const { return myVec4s.myPosition; }

	const CE_Vector3f& GetRight() const { return myVec3s.myRight; }
	const CE_Vector3f& GetUp() const { return myVec3s.myUp; }
	const CE_Vector3f& GetForward() const { return myVec3s.myForward; }

	CE_Vector3f TakePos();
	CE_Vector4f TakePos4();

	void NormalizeRotationVectors();

	CE_Matrix44f operator*=(const CE_Matrix44f& aOther);
};

inline CE_Vector3f operator*(const CE_Vector3f& aVector, const CE_Matrix44f& aMatrix)
{
	return CE_Vector3f(
		aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8],
		aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9],
		aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10]);
}

inline CE_Vector4f operator*(const CE_Vector4f& aVector, const CE_Matrix44f& aMatrix)
{
	return CE_Vector4f(
		aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8] + aVector.w * aMatrix.myMatrix[12],
		aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9] + aVector.w * aMatrix.myMatrix[13],
		aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10] + aVector.w * aMatrix.myMatrix[14],
		aVector.x * aMatrix.myMatrix[3] + aVector.y * aMatrix.myMatrix[7] + aVector.z * aMatrix.myMatrix[11] + aVector.w * aMatrix.myMatrix[15]);
}

inline bool operator==(const CE_Matrix44f& aFirstMatrix, const CE_Matrix44f& aSecondMatrix)
{
	for (unsigned char i = 0; i < 16; ++i)
	{
		if (aFirstMatrix.myMatrix[i] != aSecondMatrix.myMatrix[i])
		{
			return false;
		}
	}
	return true;
}

inline bool operator!=(const CE_Matrix44f& aFirstMatrix, const CE_Matrix44f& aSecondMatrix)
{
	return !(operator==(aFirstMatrix, aSecondMatrix));
}

inline CE_Matrix44f operator*(const CE_Matrix44f& aFirstMatrix, const CE_Matrix44f& aSecondMatrix)
{
	CE_Matrix44f result(aFirstMatrix);
	result *= aSecondMatrix;
	return result;
}

inline CE_Matrix44f CE_Transpose(const CE_Matrix44f& aMatrix)
{
	CE_Matrix44f result(aMatrix);

	result.myMatrix[1] = aMatrix.myMatrix[4];
	result.myMatrix[2] = aMatrix.myMatrix[8];
	result.myMatrix[3] = aMatrix.myMatrix[12];
	result.myMatrix[7] = aMatrix.myMatrix[13];
	result.myMatrix[11] = aMatrix.myMatrix[14];
	result.myMatrix[6] = aMatrix.myMatrix[9];

	result.myMatrix[4] = aMatrix.myMatrix[1];
	result.myMatrix[8] = aMatrix.myMatrix[2];
	result.myMatrix[12] = aMatrix.myMatrix[3];
	result.myMatrix[13] = aMatrix.myMatrix[7];
	result.myMatrix[14] = aMatrix.myMatrix[11];
	result.myMatrix[9] = aMatrix.myMatrix[6];

	return result;
}

inline CE_Matrix44f CE_InverseSimple(const CE_Matrix44f& aMatrix)
{
	CE_Matrix44f inverse(aMatrix);

	CE_Vector4<float> translation = inverse.GetPos4();
	inverse.myVec4s.myPosition = CE_Vector4<float>(0, 0, 0, 1.f);
	translation *= -1.f;
	translation.w = 1.f;
	inverse = CE_Transpose(inverse);
	translation = translation * inverse;

	inverse.myVec4s.myPosition = translation;
	return inverse;
}

inline CE_Matrix44f CE_InverseReal(const CE_Matrix44f& aMatrix)
{
	float inv[16], det;
	int i;

	inv[0] = aMatrix.myMatrix[5] * aMatrix.myMatrix[10] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[5] * aMatrix.myMatrix[11] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[9] * aMatrix.myMatrix[6] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[9] * aMatrix.myMatrix[7] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[13] * aMatrix.myMatrix[6] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[13] * aMatrix.myMatrix[7] * aMatrix.myMatrix[10];

	inv[4] = -aMatrix.myMatrix[4] * aMatrix.myMatrix[10] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[11] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[6] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[7] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[6] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[7] * aMatrix.myMatrix[10];

	inv[8] = aMatrix.myMatrix[4] * aMatrix.myMatrix[9] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[11] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[5] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[7] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[5] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[7] * aMatrix.myMatrix[9];

	inv[12] = -aMatrix.myMatrix[4] * aMatrix.myMatrix[9] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[10] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[5] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[6] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[5] * aMatrix.myMatrix[10] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[6] * aMatrix.myMatrix[9];

	inv[1] = -aMatrix.myMatrix[1] * aMatrix.myMatrix[10] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[1] * aMatrix.myMatrix[11] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[9] * aMatrix.myMatrix[2] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[9] * aMatrix.myMatrix[3] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[13] * aMatrix.myMatrix[2] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[13] * aMatrix.myMatrix[3] * aMatrix.myMatrix[10];

	inv[5] = aMatrix.myMatrix[0] * aMatrix.myMatrix[10] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[0] * aMatrix.myMatrix[11] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[2] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[3] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[2] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[3] * aMatrix.myMatrix[10];

	inv[9] = -aMatrix.myMatrix[0] * aMatrix.myMatrix[9] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[0] * aMatrix.myMatrix[11] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[1] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[3] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[1] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[3] * aMatrix.myMatrix[9];

	inv[13] = aMatrix.myMatrix[0] * aMatrix.myMatrix[9] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[0] * aMatrix.myMatrix[10] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[1] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[2] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[1] * aMatrix.myMatrix[10] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[2] * aMatrix.myMatrix[9];

	inv[2] = aMatrix.myMatrix[1] * aMatrix.myMatrix[6] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[1] * aMatrix.myMatrix[7] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[5] * aMatrix.myMatrix[2] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[5] * aMatrix.myMatrix[3] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[13] * aMatrix.myMatrix[2] * aMatrix.myMatrix[7] -
		aMatrix.myMatrix[13] * aMatrix.myMatrix[3] * aMatrix.myMatrix[6];

	inv[6] = -aMatrix.myMatrix[0] * aMatrix.myMatrix[6] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[0] * aMatrix.myMatrix[7] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[2] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[3] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[2] * aMatrix.myMatrix[7] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[3] * aMatrix.myMatrix[6];

	inv[10] = aMatrix.myMatrix[0] * aMatrix.myMatrix[5] * aMatrix.myMatrix[15] -
		aMatrix.myMatrix[0] * aMatrix.myMatrix[7] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[1] * aMatrix.myMatrix[15] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[3] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[1] * aMatrix.myMatrix[7] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[3] * aMatrix.myMatrix[5];

	inv[14] = -aMatrix.myMatrix[0] * aMatrix.myMatrix[5] * aMatrix.myMatrix[14] +
		aMatrix.myMatrix[0] * aMatrix.myMatrix[6] * aMatrix.myMatrix[13] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[1] * aMatrix.myMatrix[14] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[2] * aMatrix.myMatrix[13] -
		aMatrix.myMatrix[12] * aMatrix.myMatrix[1] * aMatrix.myMatrix[6] +
		aMatrix.myMatrix[12] * aMatrix.myMatrix[2] * aMatrix.myMatrix[5];

	inv[3] = -aMatrix.myMatrix[1] * aMatrix.myMatrix[6] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[1] * aMatrix.myMatrix[7] * aMatrix.myMatrix[10] +
		aMatrix.myMatrix[5] * aMatrix.myMatrix[2] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[5] * aMatrix.myMatrix[3] * aMatrix.myMatrix[10] -
		aMatrix.myMatrix[9] * aMatrix.myMatrix[2] * aMatrix.myMatrix[7] +
		aMatrix.myMatrix[9] * aMatrix.myMatrix[3] * aMatrix.myMatrix[6];

	inv[7] = aMatrix.myMatrix[0] * aMatrix.myMatrix[6] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[0] * aMatrix.myMatrix[7] * aMatrix.myMatrix[10] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[2] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[3] * aMatrix.myMatrix[10] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[2] * aMatrix.myMatrix[7] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[3] * aMatrix.myMatrix[6];

	inv[11] = -aMatrix.myMatrix[0] * aMatrix.myMatrix[5] * aMatrix.myMatrix[11] +
		aMatrix.myMatrix[0] * aMatrix.myMatrix[7] * aMatrix.myMatrix[9] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[1] * aMatrix.myMatrix[11] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[3] * aMatrix.myMatrix[9] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[1] * aMatrix.myMatrix[7] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[3] * aMatrix.myMatrix[5];

	inv[15] = aMatrix.myMatrix[0] * aMatrix.myMatrix[5] * aMatrix.myMatrix[10] -
		aMatrix.myMatrix[0] * aMatrix.myMatrix[6] * aMatrix.myMatrix[9] -
		aMatrix.myMatrix[4] * aMatrix.myMatrix[1] * aMatrix.myMatrix[10] +
		aMatrix.myMatrix[4] * aMatrix.myMatrix[2] * aMatrix.myMatrix[9] +
		aMatrix.myMatrix[8] * aMatrix.myMatrix[1] * aMatrix.myMatrix[6] -
		aMatrix.myMatrix[8] * aMatrix.myMatrix[2] * aMatrix.myMatrix[5];

	det = aMatrix.myMatrix[0] * inv[0] + aMatrix.myMatrix[1] * inv[4] + aMatrix.myMatrix[2] * inv[8] + aMatrix.myMatrix[3] * inv[12];

	det = 1.0f / det;

	CE_Matrix44f returnMatrix;

	for (i = 0; i < 16; i++)
		returnMatrix.myMatrix[i] = inv[i] * det;

	return returnMatrix;
}