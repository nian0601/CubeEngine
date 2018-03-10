#include "stdafx.h"
#include "CPY_CollisionEntity.h"


CPY_CollisionEntity::CPY_CollisionEntity(CE_Entity anEntity)
	: myEntity(anEntity)
{
}

void CPY_CollisionEntity::UpdateAABB(const CE_Vector3f& aPosition, const CE_Vector3f& aSize)
{
	// Will need to notify PhysicsWorld aswell when there is an SpatialStructure
	// Might need to be reinserted
	myAABB = CPY_AABB(aPosition, aSize);
}
