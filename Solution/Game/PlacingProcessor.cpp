#include "stdafx.h"
#include "PlacingProcessor.h"
#include "InventoryComponent.h"
#include "TranslationComponent.h"
#include "InputSingletonComponent.h"
#include "CreateEntitySingletonComponent.h"

PlacingProcessor::PlacingProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<TranslationComponent, InventoryComponent>>())
{
}

void PlacingProcessor::Update(float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();

	if (entities.Size() == 0)
		return;

	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();
	if (!input.ActionDown(PLACE_ENTITY))
		return;

	CreateEntitySingletonComponent& createSingleton = myWorld.GetSingletonComponent<CreateEntitySingletonComponent>();

	for (CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);

		CE_Vector3f pos = translation.myOrientation.GetPos();
		pos.z += 1.4f;

		CreateEntitySingletonComponent::Entry& entry = createSingleton.myEntries.Add();
		entry.myPosition = pos;
		entry.myEntityType = MOVER;
	}
}
