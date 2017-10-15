#include "stdafx.h"
#include "PlacingProcessor.h"
#include "InventoryComponent.h"
#include "TranslationComponent.h"
#include "InputSingletonComponent.h"
#include "CreateEntitySingletonComponent.h"
#include "SelectedEntitySingletonComponent.h"

PlacingProcessor::PlacingProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<TranslationComponent, InventoryComponent>>())
{
}

void PlacingProcessor::Update(float /*aDelta*/)
{
	SelectedEntitySingletonComponent& selectedEntity = myWorld.GetSingletonComponent<SelectedEntitySingletonComponent>();
	if (selectedEntity.myHoveredEntity == CE_Invalid_Entity)
		return;

	CE_GrowingArray<CE_Entity> entities = GetEntities();

	if (entities.Size() == 0)
		return;

	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();
	if (!input.ActionDown(PLACE_ENTITY))
		return;

	CreateEntitySingletonComponent& createSingleton = myWorld.GetSingletonComponent<CreateEntitySingletonComponent>();

	TranslationComponent& placeLocation = GetComponent<TranslationComponent>(selectedEntity.myHoveredEntity);

	if (selectedEntity.myEntityTypeOnMouse == NONE)
		return;

	for (int i = 0; i < entities.Size(); ++i)
	{
		CE_Vector3f pos = placeLocation.myOrientation.GetPos();
		pos.y += 1.f;

		CreateEntitySingletonComponent::Entry& entry = createSingleton.myEntries.Add();
		entry.myPosition = pos;
		entry.myEntityType = selectedEntity.myEntityTypeOnMouse;
	}
}
