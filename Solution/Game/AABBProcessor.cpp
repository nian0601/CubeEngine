#include "stdafx.h"
#include "AABBProcessor.h"
#include "TranslationComponent.h"
#include "AABBComponent.h"
#include "CollisionSingletonComponent.h"
#include <CPY_Intersection.h>
#include <CPY_PhysicsWorld.h>

AABBProcessor::AABBProcessor(CE_World& aWorld, CPY_PhysicsWorld& aPhysicsWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<AABBComponent, TranslationComponent>>())
	, myPhysicsWorld(aPhysicsWorld)
{
}

void AABBProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	UpdateAABBs(entities);
	UpdateCollisions(entities);
}

void AABBProcessor::EntityAdded(CE_Entity anEntity)
{
	AABBComponent& aabb = GetComponent<AABBComponent>(anEntity);
	aabb.myCollisionEntity = myPhysicsWorld.CreateCollisionEntity(anEntity);
	aabb.myCollisionEntity->myCollidesWith = aabb.myCollidesWith;
	aabb.myCollisionEntity->myCollisionLayers = aabb.myCollisionLayers;
}

void AABBProcessor::UpdateAABBs(const CE_GrowingArray<CE_Entity>& someEntities)
{
	for (const CE_Entity& entity : someEntities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		AABBComponent& aabb = GetComponent<AABBComponent>(entity);
		aabb.myCollisionEntity->UpdateAABB(translation.myOrientation.GetPos(), translation.myScale);
	}
}

void AABBProcessor::UpdateCollisions(const CE_GrowingArray<CE_Entity>& /*someEntities*/)
{
	CollisionSingletonComponent& collisionSingleton = GetSingletonComponent<CollisionSingletonComponent>();
	collisionSingleton.myCollisionPairs.RemoveAll();

	myPhysicsWorld.Update();
	const CE_GrowingArray<CPY_CollisionEntry>& collisions = myPhysicsWorld.GetCollisionEntries();
	for (const CPY_CollisionEntry& entry : collisions)
	{
		collisionSingleton.myCollisionPairs.Add({ entry.myFirstEntity->myEntity, entry.mySecondEntity->myEntity});
	}
}
