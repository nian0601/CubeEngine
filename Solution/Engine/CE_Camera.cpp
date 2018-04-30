#include "stdafx.h"
#include "CE_Camera.h"


CE_Camera::CE_Camera(const CE_Vector2i& aSize)
	: myWindowSize(aSize)
{
	const float pi = 3.14159265f;

	myProjection = CE_Matrix44f::CreateProjectionMatrixLH(0.1f, 100.f, static_cast<float>(aSize.y) / aSize.x, pi * 0.5f);
	myInvertedProjection = CE_InverseReal(myProjection);
	myOrthagonalProjection = CE_Matrix44f::CreateOrthogonalMatrixLH(static_cast<float>(aSize.x), static_cast<float>(aSize.y), 0.1f, 100.f);

	myIsDirty = true;
}


CE_Camera::~CE_Camera()
{
}

void CE_Camera::Update()
{
	if (myIsDirty)
	{
		myView = CE_InverseSimple(myOrientation);
	}
}

void CE_Camera::Rotate(const CE_Matrix44f& aRotation)
{
	CE_Vector3f pos = myOrientation.GetPos();
	myOrientation.SetPos(CE_Vector3f());
	myOrientation = myOrientation * aRotation;
	myOrientation.SetPos(pos);

	myIsDirty = true;
}

void CE_Camera::SetPosition(const CE_Vector3f& aPosition)
{
	myOrientation.SetPos(aPosition);

	myIsDirty = true;
}

void CE_Camera::Move(const CE_Vector3f& aMoveVector)
{
	CE_Vector3f pos = myOrientation.GetPos();
	myOrientation.SetPos(pos + aMoveVector);
}

CE_Vector3f CE_Camera::UnprojectPosition(const CE_Vector2f& aPosition, float aDepth) const
{
	CE_Vector2f windowSize;
	windowSize.x = static_cast<float>(myWindowSize.x);
	windowSize.y = static_cast<float>(myWindowSize.y);

	float flippedY = windowSize.y - aPosition.y;

	CE_Vector4f in;
	in.x = aPosition.x / windowSize.x * 2.f - 1.f;
	in.y = flippedY / windowSize.y * 2.f - 1.f;
	in.z = 2.f * aDepth - 1.f;
	in.w = 1.f;

	CE_Matrix44f invertedViewProjection = CE_InverseReal(myView * myProjection);

	CE_Vector4f worldPos = in * invertedViewProjection;
	if (worldPos.w == 0.f)
		return CE_Vector3f(0.f);

	worldPos.w = 1.f / worldPos.w;

	CE_Vector3f out;
	out.x = worldPos.x * worldPos.w;
	out.y = worldPos.y * worldPos.w;
	out.z = worldPos.z * worldPos.w;
	return out;
}
