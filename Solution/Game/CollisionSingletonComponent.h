#pragma once

#include <CE_BaseComponent.h>
#include <CE_CustomEntityTypes.h>

struct CollisionSingletonComponent : public CE_BaseComponent
{
	struct Pair
	{
		Pair(){}
		Pair(CE_Entity aFirst, CE_Entity aSecond) : myFirst(aFirst), mySecond(aSecond) {}
		CE_Entity myFirst;
		CE_Entity mySecond;
	};

	CE_GrowingArray<Pair> myCollisionPairs;
};
