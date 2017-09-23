#include "stdafx.h"
#include "EntityFactory.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "InventoryComponent.h"
#include "PickUpComponent.h"
#include "RotationComponent.h"
#include "MoverComponent.h"


EntityFactory::EntityFactory(CE_World& anRealWorld, CE_World& anTemplateWorld)
	: myRealWorld(anRealWorld)
	, myTemplateWorld(anTemplateWorld)
{
	LoadTemplateEntities();
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::LoadTemplateEntities()
{
	myTemplateEntityMap[GROUND] = myTemplateWorld.CreateEmptyEntity();
	myTemplateEntityMap[PLAYER] = myTemplateWorld.CreateEmptyEntity();
	myTemplateEntityMap[PICK_UP] = myTemplateWorld.CreateEmptyEntity();
	myTemplateEntityMap[MOVER] = myTemplateWorld.CreateEmptyEntity();

	LoadGround();
	LoadPlayer();
	LoadPickUp();
	LoadMover();
}

CE_Entity EntityFactory::InstansiateEntity(int anIdentifier)
{
	CE_ASSERT(myTemplateEntityMap.KeyExists(anIdentifier), "Couldnt find Entity %i to Instansiate");

	CE_Entity templateEntity = myTemplateEntityMap[anIdentifier];
	CE_Entity newEntity = myRealWorld.CreateEmptyEntity();

	CopyComponent<CollisionComponent>(templateEntity, newEntity);
	CopyComponent<MovementComponent>(templateEntity, newEntity);
	CopyComponent<InventoryComponent>(templateEntity, newEntity);
	CopyComponent<PickUpComponent>(templateEntity, newEntity);
	CopyComponent<RenderComponent>(templateEntity, newEntity);
	CopyComponent<RotationComponent>(templateEntity, newEntity);
	CopyComponent<TranslationComponent>(templateEntity, newEntity);
	CopyComponent<MoverComponent>(templateEntity, newEntity);

	return newEntity;
}

void EntityFactory::LoadGround()
{
	CE_Entity entity = myTemplateEntityMap[GROUND];

	myTemplateWorld.AddComponent<TranslationComponent>(entity);
	RenderComponent& render = myTemplateWorld.AddComponent<RenderComponent>(entity);

	float color = 0.58f;
	RenderComponent::Entry& entry = render.myEntries.Add();
	entry.myColor = CE_Vector4f(color, color, color, 1.f);
	entry.myScale = CE_Vector3f(1.f);
}

void EntityFactory::LoadPlayer()
{
	CE_Entity entity = myTemplateEntityMap[PLAYER];

	myTemplateWorld.AddComponent<TranslationComponent>(entity);
	RenderComponent& render = myTemplateWorld.AddComponent<RenderComponent>(entity);
	MovementComponent& input = myTemplateWorld.AddComponent<MovementComponent>(entity);
	CollisionComponent& collision = myTemplateWorld.AddComponent<CollisionComponent>(entity);
	myTemplateWorld.AddComponent<InventoryComponent>(entity);

	RenderComponent::Entry& entry = render.myEntries.Add();
	entry.myColor = CE_Vector4f(0.f, 0.f, 0.56f, 1.f);
	entry.myScale = CE_Vector3f(1.f);

	input.mySpeed = 10.f;
	collision.myRadius = 0.5f;
}

void EntityFactory::LoadPickUp()
{
	CE_Entity entity = myTemplateEntityMap[PICK_UP];

	myTemplateWorld.AddComponent<TranslationComponent>(entity);
	RenderComponent& render = myTemplateWorld.AddComponent<RenderComponent>(entity);
	CollisionComponent& collision = myTemplateWorld.AddComponent<CollisionComponent>(entity);
	PickUpComponent& pickup = myTemplateWorld.AddComponent<PickUpComponent>(entity);

	RenderComponent::Entry& entry = render.myEntries.Add();
	entry.myColor = CE_Vector4f(1.f, 0.f, 0.f, 1.f);
	entry.myScale = CE_Vector3f(1.f);
	collision.myRadius = 0.5f;
	pickup.myItemType = eItemType::STONE;
}

void EntityFactory::LoadMover()
{
	CE_Entity entity = myTemplateEntityMap[MOVER];

	myTemplateWorld.AddComponent<TranslationComponent>(entity);
	RenderComponent& render = myTemplateWorld.AddComponent<RenderComponent>(entity);
	CollisionComponent& collision = myTemplateWorld.AddComponent<CollisionComponent>(entity);
	MoverComponent& mover = myTemplateWorld.AddComponent<MoverComponent>(entity);

	RenderComponent::Entry& entry = render.myEntries.Add();
	entry.myColor = CE_Vector4f(0.f, 0.34f, 0.f, 1.f);
	entry.myScale = CE_Vector3f(0.15f, 0.3f, 1.f);

	CE_Vector3f offset = CalculateOffset(entry.myScale);
	entry.myOffsetMatrix.SetPos(-offset.x, -offset.y, offset.z);

	RenderComponent::Entry& entry2 = render.myEntries.Add();
	entry2.myColor = CE_Vector4f(0.f, 0.34f, 0.f, 1.f);
	entry2.myScale = CE_Vector3f(0.15f, 0.3f, 1.f);

	offset = CalculateOffset(entry2.myScale);
	entry2.myOffsetMatrix.SetPos(offset.x, -offset.y, offset.z);

	RenderComponent::Entry& entry3 = render.myEntries.Add();
	entry3.myColor = CE_Vector4f(0.f, 0.14f, 0.f, 1.f);
	entry3.myScale = CE_Vector3f(1.f, 0.15f, 1.f);

	offset = CalculateOffset(entry3.myScale);
	entry3.myOffsetMatrix.SetPos(offset.x, -offset.y, offset.z);

	collision.myRadius = 0.5f;
	mover.mySpeed = 2.f;
	mover.myDirection = CE_Vector3f(0.f, 0.f, -1.f);
}

CE_Vector3f EntityFactory::CalculateOffset(const CE_Vector3f& aScale) const
{
	CE_Vector3f offset = CE_Vector3f(1.f);
	offset -= aScale;
	offset *= 0.5f;
	return offset;
}
