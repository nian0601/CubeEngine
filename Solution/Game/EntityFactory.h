#pragma once

#include <CE_Map.h>
#include <CE_World.h>

enum eEntityTypes
{
	GROUND,
	PLAYER,
	PICK_UP,
};

class CE_World;
class EntityFactory
{
public:
	EntityFactory(CE_World& anRealWorld, CE_World& anTemplateWorld);
	~EntityFactory();

	void LoadTemplateEntities();

	CE_Entity InstansiateEntity(int anIdentifier);

private:
	void LoadGround();
	void LoadPlayer();
	void LoadPickUp();

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