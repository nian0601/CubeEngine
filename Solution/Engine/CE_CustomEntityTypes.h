#pragma once

#include "CE_GrowingArray.h"
#include "CE_StaticArray.h"

#define MAX_NUMBER_OF_COMPONENTS 32

struct CE_BaseComponent;

typedef unsigned int CE_Entity;
static const CE_Entity CE_Invalid_Entity = static_cast<unsigned int>(-1);

typedef CE_GrowingArray<CE_BaseComponent*> CE_ComponentArray;


struct CE_EntityComponentArray
{
	CE_EntityComponentArray()
		: myComponentMask(0)
	{
		for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
		{
			myComponentIndices[i] = -1;
		}
	}

	void SetComponent(int aComponentID, int aIndex) 
	{
		myComponentIndices[aComponentID] = aIndex; 
		myComponentMask |= (1 << aComponentID);
	}
	void RemoveComponent(int aComponentID) 
	{
		myComponentIndices[aComponentID] = -1;
		myComponentMask &= ~(1 << aComponentID);
	}

	bool HasComponent(int aComponentID) { return (myComponentMask & (1 << aComponentID)) > 0; }

	CE_StaticArray<int, MAX_NUMBER_OF_COMPONENTS> myComponentIndices;
	unsigned int myComponentMask;
};
