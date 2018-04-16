#include "stdafx.h"

#include "MovementComponent.h"
#include "MovementProcessor.h"
#include "TranslationComponent.h"
#include "InputSingletonComponent.h"

MovementProcessor::MovementProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<MovementComponent, TranslationComponent>>())
{
}

void MovementProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	//InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		
		CE_Vector3f direction = translation.myOrientation.GetForward();
		//if (input.ActionIsPressed(MOVE_FORWARD))
		//	direction.z += 1.f;
		//if (input.ActionIsPressed(MOVE_BACKWARD))
		//	direction.z -= 1.f;
		//if (input.ActionIsPressed(MOVE_LEFT))
		//	direction.x -= 1.f;
		//if (input.ActionIsPressed(MOVE_RIGHT))
		//	direction.x += 1.f;
		//
		//CE_Normalize(direction);

		MovementComponent& movement = GetComponent<MovementComponent>(entity);
		CE_Vector3f pos = translation.myOrientation.GetPos();
		pos += direction * movement.mySpeed * aDelta;
		translation.myOrientation.SetPos(pos);
	}
}
