#include "stdafx.h"

#include "BehaviorComponent.h"
#include "BehaviorProcessor.h"
#include "TranslationComponent.h"
#include "InputSingletonComponent.h"
#include "CursorSingletonComponent.h"
#include <CE_BlackBoard.h>

BehaviorProcessor::BehaviorProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<BehaviorComponent, TranslationComponent>>())
{
}

void BehaviorProcessor::Update(float aDelta)
{
	CE_GrowingArray<CE_Entity> entities = GetEntities();
	for (const CE_Entity& entity : entities)
	{
		BehaviorComponent& behavior = GetComponent<BehaviorComponent>(entity);
		behavior.myBehaviorTree->Update(aDelta);

		if (behavior.myBehaviorTree->IsRunning())
		{
			CE_Blackboard& blackboard = behavior.myBehaviorTree->GetBlackboard();
			CE_Vector3f selfPosition;

			if(blackboard.Get("selfPosition", selfPosition) == false)
				continue;

			TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
			translation.myOrientation.SetPos(selfPosition);
		}
	}
}
