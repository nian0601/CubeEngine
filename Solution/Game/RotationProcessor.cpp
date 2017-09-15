#include "stdafx.h"
#include "RotationProcessor.h"

#include "RotationComponent.h"
#include "TranslationComponent.h"

RotationProcessor::RotationProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<TranslationComponent, RotationComponent>>())
{
}


RotationProcessor::~RotationProcessor()
{
}

void RotationProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		RotationComponent& rotation = GetComponent<RotationComponent>(entity);

		CE_Matrix44f& orientation = translation.myOrientation;
		CE_Vector3f pos = orientation.GetPos();
		orientation.SetPos(CE_Vector3f(0.f, 0.f, 0.f));
		orientation = orientation * CE_Matrix44f::CreateRotateAroundY(rotation.mySpeed * aDelta);
		orientation.SetPos(pos);
	}
}
