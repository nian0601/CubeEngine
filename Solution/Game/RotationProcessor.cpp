#include "stdafx.h"

#include "TranslationComponent.h"
#include "RotationComponent.h"
#include "RotationProcessor.h"

RotationProcessor::RotationProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<TranslationComponent, RotationComponent>>())
{
}

void RotationProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		RotationComponent& rotation = GetComponent<RotationComponent>(entity);

		CE_Vector3f pos = translation.myOrientation.TakePos();

		CE_Vector3f scaledRotation = rotation.mySpeeds * aDelta;

		translation.myOrientation *= CE_Matrix44f::CreateRotateAroundX(scaledRotation.x);
		translation.myOrientation *= CE_Matrix44f::CreateRotateAroundY(scaledRotation.y);
		translation.myOrientation *= CE_Matrix44f::CreateRotateAroundZ(scaledRotation.z);

		translation.myOrientation.SetPos(pos);
	}
}
