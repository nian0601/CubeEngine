#pragma once

#include "CE_CustomEntityTypes.h"
#include "CE_GrowingArray.h"
#include "CE_StaticArray.h"


struct CE_BaseComponent;
struct CE_ComponentFilter;

class CE_ComponentStorage
{
public:
	CE_ComponentStorage();
	~CE_ComponentStorage();

	void AddEntity();
	void DestroyEntity(CE_Entity anEntity);
	void AddComponent(CE_Entity aEntity, CE_BaseComponent* aComponent, unsigned int aComponentID);
	void RemoveComponent(CE_Entity aEntity, unsigned int aComponentID);
	CE_BaseComponent& GetComponent(CE_Entity aEntity, unsigned int aComponentID);
	bool HasComponent(CE_Entity aEntity, unsigned int aComponentID);

	const CE_GrowingArray<CE_Entity>& GetEntities(const CE_ComponentFilter& aFilter);

	bool GetEntityComponentArray(CE_Entity aEntity, CE_EntityComponentArray& outArray) const;

private:
	CE_GrowingArray<CE_ComponentArray> myComponents;
	CE_GrowingArray<CE_EntityComponentArray> myEntityComponents;
					
	CE_GrowingArray<CE_Entity> myEntitiesToReturn;
};