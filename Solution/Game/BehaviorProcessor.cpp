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
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	CE_Vector3f selectedPosition;
	bool hasSelected = false;
	CursorSingletonComponent& selected = myWorld.GetSingletonComponent<CursorSingletonComponent>();

	if (selected.myHoveredEntity != CE_Invalid_Entity)
	{
		hasSelected = true;
		TranslationComponent& selectedTranslation = GetComponent<TranslationComponent>(selected.myHoveredEntity);
		selectedPosition = selectedTranslation.myOrientation.GetPos();
	}

	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		BehaviorComponent& behavior = GetComponent<BehaviorComponent>(entity);
		CE_Blackboard& blackboard = behavior.myBehaviorTree->GetBlackboard();

		if (input.ActionDown(LBUTTON) && hasSelected)
		{
			blackboard.myPosition = translation.myOrientation.GetPos();

			blackboard.myTargetPosition = selectedPosition;
			blackboard.myTargetPosition.y += 1.f;

			behavior.myBehaviorTree->Restart();
			behavior.myBehaviorTree->Start();
		}

		behavior.myBehaviorTree->Update(aDelta);

		if(!behavior.myBehaviorTree->IsFinished())
			translation.myOrientation.SetPos(blackboard.myPosition);
	}
}
