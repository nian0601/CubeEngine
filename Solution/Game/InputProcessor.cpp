#include "stdafx.h"

#include "InputComponent.h"
#include "InputProcessor.h"
#include "TranslationComponent.h"
#include <CE_Input.h>

InputProcessor::InputProcessor(CE_World& aWorld, CE_Input& aInput)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<InputComponent, TranslationComponent>>())
	, myInput(aInput)
{
}

void InputProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();

	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		InputComponent& input = GetComponent<InputComponent>(entity);

		CE_Vector3f direction;
		if (myInput.KeyIsPressed(DIK_W))
			direction.z += 1.f;
		if (myInput.KeyIsPressed(DIK_S))
			direction.z -= 1.f;
		if (myInput.KeyIsPressed(DIK_A))
			direction.x -= 1.f;
		if (myInput.KeyIsPressed(DIK_D))
			direction.x += 1.f;

		CE_Normalize(direction);

		CE_Vector3f pos = translation.myOrientation.GetPos();
		pos += direction * input.mySpeed * aDelta;
		translation.myOrientation.SetPos(pos);
	}
}
