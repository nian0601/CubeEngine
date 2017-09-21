#include "stdafx.h"
#include "MoverProcessor.h"

#include "CollisionSingletonComponent.h"
#include "TranslationComponent.h"
#include "MoverComponent.h"

MoverProcessor::MoverProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld)
{
}

void MoverProcessor::Update(float aDelta)
{
	CollisionSingletonComponent& collisionComponent = myWorld.GetSingletonComponent<CollisionSingletonComponent>();

	for (CollisionSingletonComponent::Pair& pair : collisionComponent.myCollisionPairs)
	{
		CE_Entity moverEntity = CE_Invalid_Entity;
		CE_Entity otherEntity = CE_Invalid_Entity;

		if (HasComponent<MoverComponent>(pair.myFirst) && HasComponent<TranslationComponent>(pair.mySecond))
		{
			moverEntity = pair.myFirst;
			otherEntity = pair.mySecond;
		}
		else if (HasComponent<MoverComponent>(pair.mySecond) && HasComponent<TranslationComponent>(pair.myFirst))
		{
			moverEntity = pair.mySecond;
			otherEntity = pair.myFirst;
		}

		if(moverEntity == CE_Invalid_Entity)
			continue;

		MoverComponent& mover = GetComponent<MoverComponent>(moverEntity);
		TranslationComponent& translation = GetComponent<TranslationComponent>(otherEntity);

		CE_Vector3f pos = translation.myOrientation.GetPos();
		pos += mover.myDirection * mover.mySpeed * aDelta;
		translation.myOrientation.SetPos(pos);
	}
}
