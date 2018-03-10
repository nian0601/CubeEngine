#pragma once
#include "CPY_AABB.h"

class CPY_CollisionEntity
{
public:
	CPY_CollisionEntity(CE_Entity anEntity);

	void UpdateAABB(const CE_Vector3f& aPosition, const CE_Vector3f& aSize);
	
	CPY_AABB myAABB;
	const CE_Entity myEntity;
	int myCollisionLayers;
	int myCollidesWith;
};

