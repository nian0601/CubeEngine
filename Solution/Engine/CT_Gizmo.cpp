#include "stdafx.h"
#include "CT_Gizmo.h"
#include "CE_RendererProxy.h"
#include "CE_Camera.h"
#include "CE_Input.h"
#include "CPY_Line3D.h"
#include "CPY_Intersection.h"

CT_Gizmo::CT_Gizmo(const CE_Camera& aCamera)
	: myCamera(aCamera)
	, myHoveredIndex(-1)
	, mySelectedIndex(-1)
	, myTransform(nullptr)
	, myScale(nullptr)
	, myAccumulatedMouseMovementThreshold(15.f)
{
	myAABBs[0].myExtents = CE_Vector3f(1.f, 0.2f, 0.2f);
	myAABBs[1].myExtents = CE_Vector3f(0.2f, 1.f, 0.2f);
	myAABBs[2].myExtents = CE_Vector3f(0.2f, 0.2f, 1.f);

	myColors[0] = CE_Vector4f(1.f, 0.f, 0.f, 1.f);
	myColors[1] = CE_Vector4f(0.f, 1.f, 0.f, 1.f);
	myColors[2] = CE_Vector4f(0.f, 0.f, 1.f, 1.f);
}

bool CT_Gizmo::Update(const CE_Input& anInput, float aDelta)
{
	if (myTransform == nullptr)
		return false;

	myHoveredIndex = GetIndexUnderMouse(anInput);

	if (anInput.MouseDown(0) && myHoveredIndex != -1)
	{
		mySelectedIndex = GetIndexUnderMouse(anInput);
		return true;
	}
	else if (anInput.MouseUp(0))
	{
		mySelectedIndex = -1;
		return false;
	}
	else if (anInput.MouseIsPressed(0) && mySelectedIndex != -1)
	{
		UpdateMouseDrag(anInput, aDelta);
		UpdateAABB();

		return true;
	}

	return false;
}

void CT_Gizmo::Render(CE_RendererProxy& aRenderProxy)
{
	if (myTransform == nullptr)
		return;

	CE_Vector3f center = myTransform->GetPos();
	for (int i = 0; i < 3; ++i)
	{
		myTransform->SetPos(myAABBs[i].myCenterPos);

		CE_Vector4f color = myColors[i];
		if (i == mySelectedIndex)
			color = CE_Vector4f(1.f, 1.f, 1.f, 1.f);
		else if (i == myHoveredIndex && mySelectedIndex == -1)
			color = CE_Vector4f(0.5f, 0.5f, 0.5f, 1.f);

		aRenderProxy.AddModel(*myTransform, myAABBs[i].myExtents, color, 0.f, 1.f);
	}

	myTransform->SetPos(center);
}

void CT_Gizmo::SetTransformAndScale(CE_Matrix44f* aTransform, CE_Vector3f* aScale)
{
	myTransform = aTransform;
	myScale = aScale;
	UpdateAABB();
}

void CT_Gizmo::UpdateMouseDrag(const CE_Input& anInput, float aDelta)
{
	float movement = anInput.GetMouseDX();
	if (abs(movement) < abs(anInput.GetMouseDY()))
		movement = -anInput.GetMouseDY();

	static float accumMovement = 0.f;
	accumMovement += abs(movement);

	bool snapMove = false;
	if (accumMovement > myAccumulatedMouseMovementThreshold)
	{
		accumMovement = 0.f;
		snapMove = true;
	}
	if (anInput.KeyIsPressed(DIK_LALT))
		UpdateScale(anInput, movement, snapMove, aDelta);
	else
		UpdateTranslation(anInput, movement, snapMove, aDelta);
}

void CT_Gizmo::UpdateTranslation(const CE_Input& anInput, float aMouseMovement, bool aCanDoSnapMove, float aDelta)
{
	CE_Vector3f direction = myTransform->GetRight();
	if (mySelectedIndex == 1)
		direction = myTransform->GetUp();
	else if (mySelectedIndex == 2)
		direction = myTransform->GetForward();

	CE_Vector3f position = myTransform->TakePos();

	if (anInput.KeyIsPressed(DIK_LSHIFT))
	{
		if(aCanDoSnapMove)
		{
			if (aMouseMovement > 0.f)
				position += direction;
			else
				position -= direction;
		}

		CE_RoundToInt(position);
	}
	else
	{
		float sensitivity = 10.f;
		position += direction * aMouseMovement * sensitivity * aDelta;
	}

	myTransform->SetPos(position);
}

void CT_Gizmo::UpdateScale(const CE_Input& anInput, float aMouseMovement, bool aCanDoSnapMove, float aDelta)
{
	if (myScale == nullptr)
		return;

	float* valueToChange = &myScale->x;
	if(mySelectedIndex == 1)
		valueToChange = &myScale->y;
	else if(mySelectedIndex == 2)
		valueToChange = &myScale->z;

	if (anInput.KeyIsPressed(DIK_LSHIFT))
	{
		if (aCanDoSnapMove)
		{
			if (aMouseMovement > 0.f)
				*valueToChange += 1.f;
			else
				*valueToChange -= 1.f;
		}

		CE_RoundToInt(*valueToChange);
	}
	else
	{
		float sensitivity = 10.f;
		*valueToChange += 1.f * aMouseMovement * sensitivity * aDelta;
	}
	
	*valueToChange = CE_Max(*valueToChange, 0.1f);
}

void CT_Gizmo::UpdateAABB()
{
	if (myTransform != nullptr)
	{
		const CE_Vector3f& center = myTransform->GetPos();
		CE_Vector3f halfScale = *myScale / 2.f;
		for (int i = 0; i < 3; ++i)
		{
			CE_Vector3f newPos = center + myTransform->myRow3s[i].myRow *  halfScale[i];
			newPos[i] += myAABBs[i].myExtents[i] * 0.5f;

			myAABBs[i].Move(newPos);
		}
	}
}

int CT_Gizmo::GetIndexUnderMouse(const CE_Input& anInput) const
{
	CE_Vector2f mousePosition = anInput.GetMousePosition();

	CE_Vector3f lineStart = myCamera.UnprojectPosition(mousePosition, 0.f);
	CE_Vector3f lineEnd = myCamera.UnprojectPosition(mousePosition, 1.f);
	CPY_Line3D line(lineStart, lineEnd);

	int index = -1;
	float bestDistance = FLT_MAX;
	for (int i = 0; i < 3; ++i)
	{
		float distance = CPY_Intersection::LineVSAABB(myAABBs[i], line);
		if (distance >= 0.f && distance < bestDistance)
		{
			bestDistance = distance;
			index = i;
		}
	}

	return index;
}
