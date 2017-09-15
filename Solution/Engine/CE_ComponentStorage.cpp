#include "stdafx.h"

#include "CE_ComponentFilter.h"
#include "CE_ComponentStorage.h"
#include "CE_BaseComponent.h"

CE_ComponentStorage::CE_ComponentStorage()
{
	myComponents.Respace(MAX_NUMBER_OF_COMPONENTS);
	myEntityComponents.Respace(128);
	myEntitiesToReturn.Respace(128);
	for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
	{
		myComponents.Add(CE_ComponentArray());
		myComponents[i].Respace(128);
	}
}


CE_ComponentStorage::~CE_ComponentStorage()
{
}

void CE_ComponentStorage::AddEntity()
{
	myEntityComponents.Add(CE_EntityComponentArray());
	CE_EntityComponentArray& components = myEntityComponents.GetLast();

	for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
	{
		components[i] = -1;
	}
}

void CE_ComponentStorage::AddComponent(CE_Entity aEntity, CE_BaseComponent* aComponent, unsigned int aComponentID)
{
	CE_ASSERT(myEntityComponents.Size() > static_cast<int>(aEntity), "Invalid Entity-ID");
	CE_ASSERT(myEntityComponents[aEntity][aComponentID] == -1, "Tried to add a component twice");

	myComponents[aComponentID].Add(aComponent);
	myEntityComponents[aEntity][aComponentID] = myComponents[aComponentID].Size() - 1;
}

void CE_ComponentStorage::RemoveComponent(CE_Entity aEntity, unsigned int aComponentID)
{
	CE_ASSERT(HasComponent(aEntity, aComponentID), "Tried to Remove an invalid component");

	int index = myEntityComponents[aEntity][aComponentID];
	CE_SAFE_DELETE(myComponents[aComponentID][index]);
	myEntityComponents[aEntity][aComponentID] = -1;
}

CE_BaseComponent& CE_ComponentStorage::GetComponent(CE_Entity aEntity, unsigned int aComponentID)
{
	CE_ASSERT(HasComponent(aEntity, aComponentID), "Tried to Get an invalid component");

	int componentIndex = myEntityComponents[aEntity][aComponentID];
	return *myComponents[aComponentID][componentIndex];
}

bool CE_ComponentStorage::HasComponent(CE_Entity aEntity, unsigned int aComponentID)
{
	if (myEntityComponents.Size() <= static_cast<int>(aEntity))
	{
		return false;
	}

	return myEntityComponents[aEntity][aComponentID] != -1;
}

const CE_GrowingArray<CE_Entity>& CE_ComponentStorage::GetEntities(const CE_ComponentFilter& aFilter)
{
	myEntitiesToReturn.RemoveAll();
	for (int i = 0; i < myEntityComponents.Size(); ++i)
	{
		if (aFilter.Compare(myEntityComponents[i]) == true)
		{
			myEntitiesToReturn.Add(i);
		}
	}

	return myEntitiesToReturn;
}

bool CE_ComponentStorage::GetEntityComponentArray(CE_Entity aEntity, CE_EntityComponentArray& outArray) const
{
	if (myEntityComponents.Size() <= static_cast<int>(aEntity))
		return false;

	outArray = myEntityComponents[aEntity];
	return true;
}