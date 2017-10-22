#pragma once

#include <CE_Map.h>
#include <CE_World.h>
#include "Types.h"

class CE_FileParser;
class CE_World;

class EntityFactory
{
public:
	EntityFactory(CE_World& anRealWorld, CE_World& anTemplateWorld);
	~EntityFactory();

	void LoadTemplateEntities();

	CE_Entity InstansiateEntity(eEntityTypes anIdentifier);

private:
	CE_Entity LoadFromDisk(const char* aFilePath);
	void LoadComponents(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadRenderComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadMovementComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadTranslateComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadInventoryComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadAABBComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadPickupComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadMoverComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadResourceComponent(CE_Entity anEntity, CE_FileParser& aFileParser);
	void LoadBehaviorComponent(CE_Entity anEntity, CE_FileParser& aFileParser);

	void LoadEmptyComponent(CE_FileParser& aFileParser);

	template<typename T>
	void CopyComponent(CE_Entity aSource, CE_Entity aTarget);

	CE_World& myRealWorld;
	CE_World& myTemplateWorld;
	CE_Map<int, CE_Entity> myTemplateEntityMap;
};

template<typename T>
void EntityFactory::CopyComponent(CE_Entity aSource, CE_Entity aTarget)
{
	if (myTemplateWorld.HasComponent<T>(aSource))
	{
		T& templateComponent = myTemplateWorld.GetComponent<T>(aSource);

		myRealWorld.AddComponent<T>(aTarget) = templateComponent;
	}
}