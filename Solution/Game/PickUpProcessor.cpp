#include "stdafx.h"
#include "PickUpProcessor.h"
#include "InventoryComponent.h"
#include "PickUpComponent.h"
#include "CollisionSingletonComponent.h"

PickUpProcessor::PickUpProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld)
{
}

void PickUpProcessor::Update(float /*aDelta*/)
{
	CollisionSingletonComponent& collisionSingleton = GetSingletonComponent<CollisionSingletonComponent>();

	for (const CollisionSingletonComponent::Pair& collisionPair : collisionSingleton.myCollisionPairs)
	{
		CE_Entity inventoryEntity = CE_Invalid_Entity;
		CE_Entity pickupEntity = CE_Invalid_Entity;

		if (HasComponent<InventoryComponent>(collisionPair.myFirst) && HasComponent<PickUpComponent>(collisionPair.mySecond))
		{
			inventoryEntity = collisionPair.myFirst;
			pickupEntity = collisionPair.mySecond;
		}
		else if (HasComponent<InventoryComponent>(collisionPair.mySecond) && HasComponent<PickUpComponent>(collisionPair.myFirst))
		{
			inventoryEntity = collisionPair.mySecond;
			pickupEntity = collisionPair.myFirst;
		}

		if(inventoryEntity == CE_Invalid_Entity || pickupEntity == CE_Invalid_Entity)
			continue;

		//InventoryComponent& inventory = GetComponent<InventoryComponent>(inventoryEntity);
		//PickUpComponent& pickup = GetComponent<PickUpComponent>(pickupEntity);

		myWorld.QueueEntityForDestruction(pickupEntity);
	}
}
