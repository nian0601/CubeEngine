#include "stdafx.h"

#include "LifetimeComponent.h"
#include "LifetimeProcessor.h"

LifetimeProcessor::LifetimeProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<LifetimeComponent>>())
{
}

void LifetimeProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		LifetimeComponent& lifetime = GetComponent<LifetimeComponent>(entity);
		lifetime.myElapsedTime += aDelta;

		if (lifetime.myElapsedTime >= lifetime.myTimeToLive)
			myWorld.QueueEntityForDestruction(entity);
	}
}
