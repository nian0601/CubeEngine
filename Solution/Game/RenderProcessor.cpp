#include "stdafx.h"
#include "RenderProcessor.h"
#include "RenderComponent.h"
#include "TranslationComponent.h"

#include <CE_RendererProxy.h>
#include "SelectedEntitySingletonComponent.h"

RenderProcessor::RenderProcessor(CE_World& aWorld, CE_RendererProxy& aRendererProxy)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<TranslationComponent, RenderComponent>>())
	, myRendererProxy(aRendererProxy)
{
}


RenderProcessor::~RenderProcessor()
{
}

void RenderProcessor::Update(float /*aDelta*/)
{
	SelectedEntitySingletonComponent& selectedEntity = myWorld.GetSingletonComponent<SelectedEntitySingletonComponent>();

	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		RenderComponent& render = GetComponent<RenderComponent>(entity);

		if (selectedEntity.myHoveredEntity == entity)
		{
			for (const RenderComponent::Entry& entry : render.myEntries)
				myRendererProxy.AddCubeData(entry.myOffsetMatrix * translation.myOrientation, entry.myScale, CE_Vector4f(0.78f, 0.78f, 0.78f, 1.f));
		}
		else
		{
			for (const RenderComponent::Entry& entry : render.myEntries)
				myRendererProxy.AddCubeData(entry.myOffsetMatrix * translation.myOrientation, entry.myScale, entry.myColor);
		}
	}
}
