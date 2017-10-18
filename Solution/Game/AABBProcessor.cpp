#include "stdafx.h"
#include "AABBProcessor.h"
#include "TranslationComponent.h"
#include "AABBComponent.h"
#include "CollisionSingletonComponent.h"
#include <CPY_Intersection.h>


AABBProcessor::AABBProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<AABBComponent, TranslationComponent>>())
{
}

void AABBProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	UpdateAABBs(entities);
	UpdateCollisions(entities);
}

void AABBProcessor::UpdateAABBs(const CE_GrowingArray<CE_Entity>& someEntities)
{
	for (const CE_Entity& entity : someEntities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		AABBComponent& aabb = GetComponent<AABBComponent>(entity);

		aabb.myAABB = CPY_AABB(translation.myOrientation.GetPos(), translation.myScale);
	}
}

void AABBProcessor::UpdateCollisions(const CE_GrowingArray<CE_Entity>& someEntities)
{
	CollisionSingletonComponent& collisionSingleton = GetSingletonComponent<CollisionSingletonComponent>();
	collisionSingleton.myCollisionPairs.RemoveAll();

	for (int outer = 0; outer < someEntities.Size() - 1; ++outer)
	{
		CE_Entity outerEntity = someEntities[outer];

		AABBComponent& outerAABB = GetComponent<AABBComponent>(outerEntity);

		for (int inner = outer + 1; inner < someEntities.Size(); ++inner)
		{
			CE_Entity innerEntity = someEntities[inner];

			AABBComponent& innerAABB = GetComponent<AABBComponent>(innerEntity);

			bool innerOuter = (outerAABB.myCollisionLayers & innerAABB.myCollidesWith) > 0;
			bool outerInner = (innerAABB.myCollisionLayers & outerAABB.myCollidesWith) > 0;
			if(innerOuter == false && outerInner == false)
				continue;

			if (CPY_Intersection::AABBvsAABB(outerAABB.myAABB, innerAABB.myAABB))
				collisionSingleton.myCollisionPairs.Add({ outerEntity, innerEntity });
		}
	}
}
