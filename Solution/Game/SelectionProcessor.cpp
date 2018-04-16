#include "stdafx.h"

#include "AABBComponent.h"
#include "SelectionProcessor.h"
#include "CursorSingletonComponent.h"
#include "InputSingletonComponent.h"

#include <CE_Camera.h>
#include <CPY_Intersection.h>
#include "TranslationComponent.h"
#include "AIEventSingletonComponent.h"
#include <CE_DebugDraw.h>
#include <CPY_PhysicsWorld.h>
#include "CreateEntitySingletonComponent.h"

SelectionProcessor::SelectionProcessor(CE_World& aWorld, const CE_Camera& aCamera, CPY_PhysicsWorld& aPhysicsWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<AABBComponent, TranslationComponent>>())
	, myCamera(aCamera)
	, myPhysicsWorld(aPhysicsWorld)
{
}

void SelectionProcessor::Update(float /*aDelta*/)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();
	CursorSingletonComponent& cursorComponent = myWorld.GetSingletonComponent<CursorSingletonComponent>();

	CE_Vector3f intersectionPoint;
	CE_Entity entityUnderMouse = FindEntityUnderMouse(input.myMousePosition, intersectionPoint);
	cursorComponent.myHoveredEntity = entityUnderMouse;

	if (entityUnderMouse == CE_Invalid_Entity)
		return;

	CE_DRAW_LINE_COLOR(intersectionPoint, intersectionPoint + CE_Vector3f(0.f, 2.f, 0.f), CE_Vector4f(0.f, 0.f, 0.f, 1.f));

	if (input.ActionDown(LBUTTON))
	{
		AIEventSingletonComponent& aiEvents = myWorld.GetSingletonComponent<AIEventSingletonComponent>();
		AIEventSingletonComponent::AIEvent& event = aiEvents.myEvents.Add();
		event.myPosition = intersectionPoint;
	}
	else if (input.ActionDown(RBUTTON))
	{
		intersectionPoint.y += 0.5f;
		CreateEntitySingletonComponent& createComponent = myWorld.GetSingletonComponent<CreateEntitySingletonComponent>();
		CE_Vector3f forward = CE_GetNormalized(intersectionPoint);
		CE_Vector3f up(0.f, 1.f, 0.f);
		CE_Vector3f right = CE_Cross(forward, up);
		CE_Normalize(right);

		CE_Matrix44f orientation;
		orientation.SetForward(forward);
		orientation.SetUp(up);
		orientation.SetRight(right);
		orientation.SetPos({ 0.f, 1.f, 0.f });
		CreateEntitySingletonComponent::Entry& entry = createComponent.myEntries.Add();
		entry.myEntityType = eEntityTypes::PROJECTILE;
		entry.myOrientation = orientation;
	}
}

CE_Entity SelectionProcessor::FindEntityUnderMouse(const CE_Vector2f& aMousePosition, CE_Vector3f& aIntersectionPointOut)
{
	CE_Vector3f lineStart = Unproject(aMousePosition, 0.f);
	CE_Vector3f lineEnd = Unproject(aMousePosition, 1.f);

	const CPY_CollisionEntity* collisionEntity = myPhysicsWorld.RayCast(lineStart, lineEnd, CollisionLayer::CLICKABLE, aIntersectionPointOut);
	if (collisionEntity)
		return collisionEntity->myEntity;

	return CE_Invalid_Entity;
}

CE_Vector3f SelectionProcessor::Unproject(const CE_Vector2f& aPosition, float aDepth) const
{
	CE_Vector2f windowSize;
	windowSize.x = static_cast<float>(myCamera.GetWindowSize().x);
	windowSize.y = static_cast<float>(myCamera.GetWindowSize().y);

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