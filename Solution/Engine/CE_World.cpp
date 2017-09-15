#include "stdafx.h"

#include "CE_BaseProcessor.h"
#include "CE_World.h"


CE_World::CE_World()
	: myNextEntity(0)
	, myComponentStorage(new CE_ComponentStorage)
{
}


CE_World::~CE_World()
{
	CE_SAFE_DELETE(myComponentStorage);
	myProcessors.DeleteAll();
}

CE_Entity CE_World::CreateEntity()
{
	CE_Entity newEntity = myNextEntity++;

	myComponentStorage->AddEntity();

	return newEntity;
}

void CE_World::Update(float aDelta)
{
	for (CE_BaseProcessor* processor : myProcessors)
	{
		processor->Update(aDelta);
	}
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
			bool matchedAfter = filter.Compare(modifiedComponents);
			modifiedComponents[aComponentID] = -1;

			bool matchedBefore = filter.Compare(modifiedComponents);

			if (matchedBefore && !matchedAfter)
				wasRemoved = true;

			if (!matchedBefore && matchedAfter)
				wasAdded = true;
		}
		else
		{
			bool matchedBefore = filter.Compare(modifiedComponents);
			modifiedComponents[aComponentID] = -1;

			bool matchedAfter = filter.Compare(modifiedComponents);

			if (matchedBefore && !matchedAfter)
				wasRemoved = true;

			if (!matchedBefore && matchedAfter)
				wasAdded = true;
		}

		if (wasRemoved)
			processor->EntityRemoved(aEntity);
	}
}