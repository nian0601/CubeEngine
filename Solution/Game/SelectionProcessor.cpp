#include "stdafx.h"

#include "AABBComponent.h"
#include "SelectionProcessor.h"
#include "SelectedEntitySingletonComponent.h"
#include "InputSingletonComponent.h"

#include <CE_Camera.h>
#include <CPY_Intersection.h>

SelectionProcessor::SelectionProcessor(CE_World& aWorld, const CE_Camera& aCamera)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<AABBComponent>>())
	, myCamera(aCamera)
{
}

void SelectionProcessor::Update(float /*aDelta*/)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();
	SelectedEntitySingletonComponent& selectedEntity = myWorld.GetSingletonComponent<SelectedEntitySingletonComponent>();

	CE_Entity entityUnderMouse = FindEntityUnderMouse(input.myMousePosition);
	selectedEntity.myHoveredEntity = entityUnderMouse;

	if (input.ActionDown(TOOL_BTN_1))
		selectedEntity.myEntityTypeOnMouse = eEntityTypes::NONE;
	else if (input.ActionDown(TOOL_BTN_2))
		selectedEntity.myEntityTypeOnMouse = eEntityTypes::MOVER;
	else if (input.ActionDown(TOOL_BTN_3))
		selectedEntity.myEntityTypeOnMouse = eEntityTypes::PICK_UP;
	else if (input.ActionDown(TOOL_BTN_4))
		selectedEntity.myEntityTypeOnMouse = eEntityTypes::PLAYER;

	if (input.ActionDown(LBUTTON))
	{
		//selectedEntity.mySelectedEntity = CE_Invalid_Entity;
	}
	else if (input.ActionDown(RBUTTON))
	{
		selectedEntity.mySelectedEntity = CE_Invalid_Entity;
	}
}

CE_Entity SelectionProcessor::FindEntityUnderMouse(const CE_Vector2f& aMousePosition)
{
	CE_Vector3f lineStart = Unproject(aMousePosition, 0.f);
	CE_Vector3f lineEnd = Unproject(aMousePosition, 1.f);

	CPY_Line3D line(lineStart, lineEnd);
	CE_Entity bestEntity = CE_Invalid_Entity;
	float bestDist = FLT_MAX;


	CE_Vector3f intersectionPoint;
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		AABBComponent& aabb = GetComponent<AABBComponent>(entity);

		if (CPY_Intersection::LineVSAABB(aabb.myAABB, line, &intersectionPoint))
		{
			float dist = CE_Length2(intersectionPoint - lineStart);
			if (dist < bestDist)
			{
				bestDist = dist;
				bestEntity = entity;
			}
		}
	}

	return bestEntity;
}

CE_Vector3f SelectionProcessor::Unproject(const CE_Vector2f& aPosition, float aDepth) const
{
	const CE_Vector2f windowSize(1280.f, 720.f);

	float flippedY = windowSize.y - aPosition.y;

	CE_Vector4f in;
	in.x = aPosition.x / windowSize.x * 2.f - 1.f;
	in.y = flippedY / windowSize.y * 2.f - 1.f;
	in.z = 2.f * aDepth - 1.f;
	in.w = 1.f;

	CE_Matrix44f invertedViewProjection = CE_InverseReal(myCamera.GetView() * myCamera.GetProjection());

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