#include "stdafx.h"
#include "CPY_PhysicsWorld.h"
#include "CPY_CollisionEntity.h"
#include "CPY_Intersection.h"


CPY_PhysicsWorld::CPY_PhysicsWorld()
{
}


CPY_PhysicsWorld::~CPY_PhysicsWorld()
{
	myCollisionEntities.DeleteAll();
}

void CPY_PhysicsWorld::Update()
{
	myCollisionEntries.RemoveAll();
	int numEntities = myCollisionEntities.Size();
	for (int outer = 0; outer < numEntities - 1; ++outer)
	{
		CPY_CollisionEntity* outerEntity = myCollisionEntities[outer];

		for (int inner = outer + 1; inner < numEntities; ++inner)
		{
			CPY_CollisionEntity* innerEntity = myCollisionEntities[inner];

			bool innerOuter = (outerEntity->myCollisionLayers & innerEntity->myCollidesWith) > 0;
			bool outerInner = (innerEntity->myCollisionLayers & outerEntity->myCollidesWith) > 0;
			if (innerOuter == false && outerInner == false)
				continue;

			if (CPY_Intersection::AABBvsAABB(outerEntity->myAABB, innerEntity->myAABB))
				myCollisionEntries.Add({ outerEntity, innerEntity });
		}
	}
}

CPY_CollisionEntity* CPY_PhysicsWorld::CreateCollisionEntity(CE_Entity anEntity)
{
	CPY_CollisionEntity* entity = new CPY_CollisionEntity(anEntity);
	myCollisionEntities.Add(entity);
	return entity;
}

const CPY_CollisionEntity* CPY_PhysicsWorld::RayCast(const CE_Vector3f& aRayStart, const CE_Vector3f& aRayEnd, int someCollisionFlags, CE_Vector3f& aIntersectionPointOut) const
{
	CPY_Line3D line(aRayStart, aRayEnd);
	const CPY_CollisionEntity* bestEntity = nullptr;
	CE_Vector3f bestIntersectionPoint;
	float bestDist = FLT_MAX;

	CE_Vector3f intersectionPoint;
	for (const CPY_CollisionEntity* entity : myCollisionEntities)
	{
		if ((entity->myCollisionLayers & someCollisionFlags) == 0)
			continue;

		if (CPY_Intersection::LineVSAABB(entity->myAABB, line, &intersectionPoint))
		{
			float dist = CE_Length2(intersectionPoint - aRayStart);
			if (dist < bestDist)
			{
				bestDist = dist;
				bestEntity = entity;
				bestIntersectionPoint = intersectionPoint;
			}
		}
	}

	aIntersectionPointOut = bestIntersectionPoint;
	return bestEntity;
}
