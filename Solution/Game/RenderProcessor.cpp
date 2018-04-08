#include "stdafx.h"
#include "RenderProcessor.h"
#include "RenderComponent.h"
#include "TranslationComponent.h"

#include <CE_RendererProxy.h>
#include "CursorSingletonComponent.h"

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
	CursorSingletonComponent& selectedEntity = myWorld.GetSingletonComponent<CursorSingletonComponent>();

	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		RenderComponent& render = GetComponent<RenderComponent>(entity);

		bool isHovered = selectedEntity.myHoveredEntity == entity;

		for (const RenderComponent::Entry& entry : render.myEntries)
		{
			CE_Vector4f color = entry.myColor;
			if (isHovered)
				color = CE_Vector4f(0.78f, 0.78f, 0.78f, entry.myMetalness);

			bool isSphere = entry.myType == ModelType::eType::SPHERE;
			myRendererProxy.AddModelData(entry.myOffsetMatrix * translation.myOrientation, entry.myScale * translation.myScale, color, entry.myMetalness, entry.myRoughness, isSphere);
		}
			
	}

	myRendererProxy.AddTextData("This is some text!", { 500.f, 400.f });
	myRendererProxy.AddSpriteData({ 700.f, 200.f }, { 50.f, 50.f }, { 1.f, 0.f, 1.f, 1.f });
}
