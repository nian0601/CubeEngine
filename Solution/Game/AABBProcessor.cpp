#include "stdafx.h"
#include "AABBProcessor.h"
#include "TranslationComponent.h"
#include "AABBComponent.h"


AABBProcessor::AABBProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<AABBComponent, TranslationComponent>>())
{
}

void AABBProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		AABBComponent& aabb = GetComponent<AABBComponent>(entity);

		aabb.myAABB = CPY_AABB(translation.myOrientation.GetPos(), translation.myScale);
	}
}
