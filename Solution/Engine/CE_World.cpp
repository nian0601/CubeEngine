#include "stdafx.h"

#include "CE_BaseProcessor.h"
#include "CE_BaseComponent.h"
#include "CE_World.h"


CE_World::CE_World()
	: myNextEntity(0)
	, myComponentStorage(new CE_ComponentStorage())
{
	mySingletonComponents.Respace(MAX_NUMBER_OF_COMPONENTS);
	for (int i = 0; i < MAX_NUMBER_OF_COMPONENTS; ++i)
	{
		mySingletonComponents.Add(nullptr);
	}
}


CE_World::~CE_World()
{
	CE_SAFE_DELETE(myComponentStorage);
	myProcessors.DeleteAll();
	mySingletonComponents.DeleteAll();
}

CE_Entity CE_World::CreateEmptyEntity()
{
	CE_Entity newEntity = myNextEntity++;

	myComponentStorage->AddEntity();

	return newEntity;
}

void CE_World::DestroyAllEntities()
{
	myNextEntity -= 1;
	while (myNextEntity != CE_Invalid_Entity)
	{
		myComponentStorage->DestroyEntity(myNextEntity);
		--myNextEntity;
	}

	myNextEntity = 0;
}

void CE_World::QueueEntityForDestruction(CE_Entity anEntity)
{
	myEntitiesToDestroy.AddUnique(anEntity);
}

void CE_World::Update(float aDelta)
{
	for (CE_BaseProcessor* processor : myProcessors)
	{
		processor->Update(aDelta);
	}

	for (const CE_Entity& entity : myEntitiesToDestroy)
	{
		myComponentStorage->DestroyEntity(entity);
	}
	myEntitiesToDestroy.RemoveAll();
}

const CE_GrowingArray<CE_Entity>& CE_World::GetEntities(const CE_ComponentFilter& aFilter)
{
	return myComponentStorage->GetEntities(aFilter);
}

void CE_World::SendEvent(const CE_Event& aEvent)
{
	for each (CE_BaseProcessor* processor in myProcessors)
	{
		processor->OnEvent(aEvent);
	}
}

void CE_World::ModifiedEntity(CE_Entity aEntity, int aComponentID, bool aAddedComponent)
{
	CE_EntityComponentArray components;
	if (!myComponentStorage->GetEntityComponentArray(aEntity, components))
		return;


	for each (CE_BaseProcessor* processor in myProcessors)
	{
		CE_EntityComponentArray modifiedComponents = components;
		const CE_ComponentFilter& filter = processor->GetComponentFilter();

		bool wasRemoved = false;
		bool wasAdded = false;

		if (aAddedComponent)
		{
			bool matchedAfter = filter.Compare(modifiedComponents.myComponentMask);
			modifiedComponents.RemoveComponent(aComponentID);
			//modifiedComponents[aComponentID] = -1;

			bool matchedBefore = filter.Compare(modifiedComponents.myComponentMask);

			if (matchedBefore && !matchedAfter)
				wasRemoved = true;

			if (!matchedBefore && matchedAfter)
				wasAdded = true;
		}
		else
		{
			bool matchedBefore = filter.Compare(modifiedComponents.myComponentMask);
			modifiedComponents.RemoveComponent(aComponentID);
			//modifiedComponents[aComponentID] = -1;

			bool matchedAfter = filter.Compare(modifiedComponents.myComponentMask);

			if (matchedBefore && !matchedAfter)
				wasRemoved = true;

			if (!matchedBefore && matchedAfter)
				wasAdded = true;
		}

		if (wasAdded)
			processor->EntityAdded(aEntity);

		if (wasRemoved)
			processor->EntityRemoved(aEntity);
	}
}