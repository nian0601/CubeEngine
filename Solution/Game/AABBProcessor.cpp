#include "stdafx.h"
#include "AABBProcessor.h"
#include "TranslationComponent.h"
#include "CollisionComponent.h"


AABBProcessor::AABBProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<CollisionComponent, TranslationComponent>>())
{
}

void AABBProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		CollisionComponent& collision = GetComponent<CollisionComponent>(entity);

		collision.myAABB = CPY_AABB(translation.myOrientation.GetPos(), translation.myScale);
	}
}
