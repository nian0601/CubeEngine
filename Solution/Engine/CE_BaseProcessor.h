#pragma once
#include "CE_ComponentFilter.h"
#include "CE_World.h"


struct CE_Event;
class CE_BaseProcessor
{
public:
	CE_BaseProcessor(CE_World& aWorld);
	CE_BaseProcessor(CE_World& aWorld, const CE_ComponentFilter& aFilter);
	virtual ~CE_BaseProcessor();

	virtual void Update(float aDelta) = 0;

	const CE_GrowingArray<CE_Entity>& GetEntities();

	template<typename T>
	void AddComponent(CE_Entity aEntity);

	template<typename T>
	void RemoveComponent(CE_Entity aEntity);

	template<typename T>
	T& GetComponent(CE_Entity aEntity);

	template<typename T>
	bool HasComponent(CE_Entity aEntity);

	template<typename T>
	T& GetSingletonComponent();

	virtual void OnEvent(const CE_Event& aEvent);

	const CE_ComponentFilter& GetComponentFilter() const { return myComponentFilter; }

	virtual void EntityAdded(CE_Entity /*aEntity*/) {};
	virtual void EntityRemoved(CE_Entity /*aEntity*/) {};

protected:
	CE_World& myWorld;

private:
	void operator=(CE_BaseProcessor&) = delete;
	CE_ComponentFilter myComponentFilter;
};

template<typename T>
void CE_BaseProcessor::AddComponent(CE_Entity aEntity)
{
	myWorld.AddComponent<T>(aEntity);
}

template<typename T>
void CE_BaseProcessor::RemoveComponent(CE_Entity aEntity)
{
	myWorld.RemoveComponent<T>(aEntity);
}

template<typename T>
T& CE_BaseProcessor::GetComponent(CE_Entity aEntity)
{
	return myWorld.GetComponent<T>(aEntity);
}

template<typename T>
bool CE_BaseProcessor::HasComponent(CE_Entity aEntity)
{
	return myWorld.HasComponent<T>(aEntity);
}

template<typename T>
T& CE_BaseProcessor::GetSingletonComponent()
{
	return myWorld.GetSingletonComponent<T>();
}