#pragma once

#include <CE_BaseComponent.h>
#include <CPY_CollisionEntity.h>
#include "Types.h"

struct AABBComponent : public CE_BaseComponent
{
	void operator=(const AABBComponent& aOther)
	{
		myAABB = aOther.myAABB;
		myCollisionLayers = aOther.myCollisionLayers;
		myCollidesWith = aOther.myCollidesWith;
	}

	CPY_CollisionEntity* myCollisionEntity;
	CollisionLayer::eLayer myCollisionLayers;
	CollisionLayer::eLayer myCollidesWith;
};
