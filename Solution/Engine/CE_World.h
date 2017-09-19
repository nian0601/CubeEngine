#pragma once

#include "CE_ComponentStorage.h"
#include "CE_CustomEntityTypes.h"
#include "CE_TypeID.h"
#include "CE_GrowingArray.h"

struct CE_ComponentFilter;
struct CE_Event;

class CE_BaseProcessor;
class CE_World
{
public:
	CE_World();
	~CE_World();

	CE_Entity CreateEmptyEntity();

	void QueueEntityForDestruction(CE_Entity anEntity);
	void Update(float aDelta);
	const CE_GrowingArray<CE_Entity>& GetEntities(const CE_ComponentFilter& aFilter);

	template<typename T>
	T& AddComponent(CE_Entity aEntity);

	template<typename T>
	void RemoveComponent(CE_Entity aEntity);

	template<typename T>
	T& GetComponent(CE_Entity aEntity);

	template<typename T>
	bool HasComponent(CE_Entity aEntity);

	template<typename T>
	void AddProcessor();

	template<typename T>
	void AddProcessor(T* aProcessor);

	template<typename T>
	T& GetSingletonComponent();

	void SendEvent(const CE_Event& aEvent);

	void ModifiedEntity(CE_Entity aEntity, int aComponentID, bool aAddedComponent);

private:
	CE_Entity myNextEntity;

	CE_ComponentStorage* myComponentStorage;
	CE_GrowingArray<CE_BaseProcessor*> myProcessors;
	CE_GrowingArray<CE_BaseComponent*> mySingletonComponents;
	CE_GrowingArray<CE_Entity> myEntitiesToDestroy;
};


template<typename T>
T& CE_World::AddComponent(CE_Entity aEntity)
{
	T* component = new T();
	unsigned int componentID = CE_TypeID<CE_BaseComponent>::GetID<T>();
	myComponentStorage->AddComponent(aEntity, component, componentID);
	ModifiedEntity(aEntity, componentID, true);

	return *component;
}

template<typename T>
void CE_World::RemoveComponent(CE_Entity aEntity)
{
	unsigned int componentID = CE_TypeID<CE_BaseComponent>::GetID<T>();
	ModifiedEntity(aEntity, componentID, false);
	myComponentStorage->RemoveComponent(aEntity, componentID);
}

template<typename T>
T& CE_World::GetComponent(CE_Entity aEntity)
{
	return static_cast<T&>(myComponentStorage->GetComponent(aEntity, CE_TypeID<CE_BaseComponent>::GetID<T>()));
}

template<typename T>
bool CE_World::HasComponent(CE_Entity aEntity)
{
	return myComponentStorage->HasComponent(aEntity, CE_TypeID<CE_BaseComponent>::GetID<T>());
}

template<typename T>
void CE_World::AddProcessor()
{
	myProcessors.Add(new T(*this));
}

template<typename T>
void CE_World::AddProcessor(T* aProcessor)
{
	myProcessors.Add(aProcessor);
}

template<typename T>
T& CE_World::GetSingletonComponent()
{
	unsigned int componentID = CE_TypeID<CE_BaseComponent>::GetID<T>();
	if (mySingletonComponents[componentID] == nullptr)
		mySingletonComponents[componentID] = new T();

	return static_cast<T&>(*mySingletonComponents[componentID]);
}