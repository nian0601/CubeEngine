#include "stdafx.h"

#include "CollisionComponent.h"
#include "CollisionProcessor.h"
#include "TranslationComponent.h"

#include "CollisionSingletonComponent.h"

CollisionProcessor::CollisionProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<CollisionComponent, TranslationComponent>>())
{
}

void CollisionProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();

	CollisionSingletonComponent& collisionSingleton = GetSingletonComponent<CollisionSingletonComponent>();
	collisionSingleton.myCollisionPairs.RemoveAll();

	for (int outer = 0; outer < entities.Size()-1; ++outer)
	{
		CE_Entity outerEntity = entities[outer];

		CollisionComponent& outerCollision = GetComponent<CollisionComponent>(outerEntity);
		TranslationComponent& outerTranslation = GetComponent<TranslationComponent>(outerEntity);

		const CE_Vector3f& outerPos = outerTranslation.myOrientation.GetPos();

		for (int inner = outer+1; inner < entities.Size(); ++inner)
		{
			CE_Entity innerEntity = entities[inner];

			CollisionComponent& innerCollision = GetComponent<CollisionComponent>(innerEntity);
			TranslationComponent& innerTranslation = GetComponent<TranslationComponent>(innerEntity);

			const CE_Vector3f& innerPos = innerTranslation.myOrientation.GetPos();

			float distance = CE_Length2(outerPos - innerPos);
			float radius = (innerCollision.myRadius + outerCollision.myRadius) * (innerCollision.myRadius + outerCollision.myRadius);

			if (distance <= radius)
			{
				collisionSingleton.myCollisionPairs.Add({ outerEntity, innerEntity });
			}
		}
	}
}
