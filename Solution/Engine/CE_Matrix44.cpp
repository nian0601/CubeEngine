#pragma once
#include "stdafx.h"
#include "CE_Matrix44.h"

CE_Matrix44f::CE_Matrix44f()
{
	for (unsigned char i = 0; i < 16; ++i)
	{
		myMatrix[i] = 0.f;
	}

	myMatrix[0] = 1.f;
	myMatrix[5] = 1.f;
	myMatrix[10] = 1.f;
	myMatrix[15] = 1.f;
}

CE_Matrix44f::CE_Matrix44f(const CE_Matrix44f& aMatrix)
{
	for (unsigned char i = 0; i < 16; ++i)
	{
		myMatrix[i] = aMatrix.myMatrix[i];
	}
}

CE_Matrix44f CE_Matrix44f::CreateRotateAroundX(float aAngleInRadians)
{
	CE_Matrix44f M;

	M.myMatrix[5] = cos(aAngleInRadians);
	M.myMatrix[6] = sin(aAngleInRadians);
	M.myMatrix[9] = -sin(aAngleInRadians);
	M.myMatrix[10] = cos(aAngleInRadians);

	return M;
}

CE_Matrix44f CE_Matrix44f::CreateRotateAroundY(float aAngleInRadians)
{
	CE_Matrix44f M;

	M.myMatrix[0] = cos(aAngleInRadians);
	M.myMatrix[2] = -sin(aAngleInRadians);
	M.myMatrix[8] = sin(aAngleInRadians);
	M.myMatrix[10] = cos(aAngleInRadians);

	return M;
}

CE_Matrix44f CE_Matrix44f::CreateRotateAroundZ(float aAngleInRadians)
{
	CE_Matrix44f M;

	M.myMatrix[0] = cos(aAngleInRadians);
	M.myMatrix[1] = sin(aAngleInRadians);
	M.myMatrix[4] = -sin(aAngleInRadians);
	M.myMatrix[5] = cos(aAngleInRadians);

	return M;
}

CE_Matrix44f CE_Matrix44f::CreateReflectionMatrixAboutAxis(const CE_Vector3f& aReflectionVector)
{
	CE_Matrix44f reflectionMatrix;
	reflectionMatrix.myMatrix[0] = 1 - 2 * (aReflectionVector.x*aReflectionVector.x);
	reflectionMatrix.myMatrix[1] = -2 * (aReflectionVector.x*aReflectionVector.y);
	reflectionMatrix.myMatrix[2] = -2 * (aReflectionVector.x*aReflectionVector.z);

	reflectionMatrix.myMatrix[4] = -2 * (aReflectionVector.y*aReflectionVector.x);
	reflectionMatrix.myMatrix[5] = 1 - 2 * (aReflectionVector.y*aReflectionVector.y);
	reflectionMatrix.myMatrix[6] = -2 * (aReflectionVector.y*aReflectionVector.z);

	reflectionMatrix.myMatrix[8] = -2 * (aReflectionVector.z*aReflectionVector.x);
	reflectionMatrix.myMatrix[9] = -2 * (aReflectionVector.z*aReflectionVector.y);
	reflectionMatrix.myMatrix[10] = 1 - 2 * (aReflectionVector.z*aReflectionVector.z);
	return reflectionMatrix;
};

CE_Matrix44f CE_Matrix44f::CreateProjectionMatrixLH(float aNearZ, float aFarZ, float aAspectRatio, float aFovAngle)
{
	CE_Matrix44f temp;
	float SinFov;
	float CosFov;
	float Height;
	float Width;

	SinFov = sin(0.5f * aFovAngle);
	CosFov = cos(0.5f * aFovAngle);

	Width = CosFov / SinFov;
	Height = Width / aAspectRatio;

	float scaling = aFarZ / (aFarZ - aNearZ);

	temp.myMatrix[0] = Width;
	temp.myMatrix[5] = Height;
	temp.myMatrix[10] = scaling;
	temp.myMatrix[11] = 1.0f;

	temp.myMatrix[14] = -scaling * aNearZ;
	temp.myMatrix[15] = 0.0f;
	return temp;
}

CE_Matrix44f CE_Matrix44f::CreateOrthogonalMatrixLH(float aWidth, float aHeight, float aNearZ, float aFarZ)
{
	CE_Matrix44f temp;

	temp.myMatrix[0] = 2.f / aWidth;
	temp.myMatrix[5] = 2.f / aHeight;
	temp.myMatrix[10] = 1.f / (aFarZ - aNearZ);
	temp.myMatrix[14] = aNearZ / (aNearZ - aFarZ);
	temp.myMatrix[15] = 1.f;

	return temp;
}

CE_Vector3f CE_Matrix44f::TakePos()
{
	CE_Vector3f pos = CE_Vector3f(myMatrix[12], myMatrix[13], myMatrix[14]);
	myVec3s.myPosition = CE_Vector3f();
	return pos;
}

CE_Vector4f CE_Matrix44f::TakePos4()
{
	CE_Vector4f pos = myVec4s.myPosition;
	myVec4s.myPosition = CE_Vector4f();
	return pos;
}

void CE_Matrix44f::NormalizeRotationVectors()
{
	CE_Normalize(myVec3s.myRight);
	CE_Normalize(myVec3s.myUp);
	CE_Normalize(myVec3s.myForward);
}

CE_Matrix44f CE_Matrix44f::operator*=(const CE_Matrix44f& aSecondMatrix)
{
	CE_Matrix44f originalFirst(*this);

	myMatrix[0] =
		originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[0] +
		originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[4] +
		originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[8] +
		originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[12];
	myMatrix[1] =
		originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[1] +
		originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[5] +
		originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[9] +
		originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[13];
	myMatrix[2] =
		originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[2] +
		originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[6] +
		originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[10] +
		originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[14];
	myMatrix[3] =
		originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[3] +
		originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[7] +
		originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[11] +
		originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[15];

	myMatrix[4] =
		originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[0] +
		originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[4] +
		originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[8] +
		originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[12];
	myMatrix[5] =
		originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[1] +
		originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[5] +
		originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[9] +
		originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[13];
	myMatrix[6] =
		originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[2] +
		originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[6] +
		originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[10] +
		originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[14];
	myMatrix[7] =
		originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[3] +
		originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[7] +
		originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[11] +
		originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[15];

	myMatrix[8] =
		originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[0] +
		originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[4] +
		originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[8] +
		originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[12];
	myMatrix[9] =
		originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[1] +
		originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[5] +
		originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[9] +
		originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[13];
	myMatrix[10] =
		originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[2] +
		originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[6] +
		originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[10] +
		originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[14];
	myMatrix[11] =
		originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[3] +
		originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[7] +
		originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[11] +
		originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[15];

	myMatrix[12] =
		originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[0] +
		originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[4] +
		originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[8] +
		originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[12];
	myMatrix[13] =
		originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[1] +
		originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[5] +
		originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[9] +
		originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[13];
	myMatrix[14] =
		originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[2] +
		originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[6] +
		originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[10] +
		originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[14];
	myMatrix[15] =
		originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[3] +
		originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[7] +
		originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[11] +
		originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[15];

	return *this;
}