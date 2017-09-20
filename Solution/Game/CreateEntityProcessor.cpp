#include "stdafx.h"
#include "CreateEntityProcessor.h"
#include "CreateEntitySingletonComponent.h"
#include "EntityFactory.h"
#include "TranslationComponent.h"

CreateEntityProcessor::CreateEntityProcessor(CE_World& aWorld, EntityFactory& aEntityFactory)
	: CE_BaseProcessor(aWorld)
	, myEntityFactory(aEntityFactory)
{
}

void CreateEntityProcessor::Update(float /*aDelta*/)
{
	CreateEntitySingletonComponent& createComponent = myWorld.GetSingletonComponent<CreateEntitySingletonComponent>();

	for (const CreateEntitySingletonComponent::Entry& entry : createComponent.myEntries)
	{
		CE_Entity newEntity = myEntityFactory.InstansiateEntity(entry.myEntityType);

		TranslationComponent& translation = GetComponent<TranslationComponent>(newEntity);
		translation.myOrientation.SetPos(entry.myPosition);
	}

	createComponent.myEntries.RemoveAll();
}

