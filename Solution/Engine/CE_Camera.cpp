#include "stdafx.h"
#include "CE_Camera.h"


CE_Camera::CE_Camera(const CE_Vector2i& aSize)
{
	const float pi = 3.14159265f;

	myProjection = CE_Matrix44f::CreateProjectionMatrixLH(0.1f, 100.f, static_cast<float>(aSize.y) / aSize.x, pi * 0.5f);
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
