#include "stdafx.h"
#include "AIEventProcessor.h"
#include "BehaviorComponent.h"
#include "AIEventSingletonComponent.h"
#include "TranslationComponent.h"
#include <CE_Blackboard.h>


AIEventProcessor::AIEventProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<BehaviorComponent, TranslationComponent>>())
{
}


void AIEventProcessor::Update(float /*aDelta*/)
{
	AIEventSingletonComponent& aiEvents = myWorld.GetSingletonComponent<AIEventSingletonComponent>();
	if (aiEvents.myEvents.Size() == 0)
		return;

	CE_GrowingArray<CE_Entity> agents = GetEntities();
	for (CE_Entity agent : agents)
	{
		BehaviorComponent& behavior = myWorld.GetComponent<BehaviorComponent>(agent);
		if(behavior.myBehaviorTree->IsRunning())
			continue;

		if (aiEvents.myEvents.Size() == 0)
			return;

		const AIEventSingletonComponent::AIEvent& event = aiEvents.myEvents[0];

		CE_Blackboard& blackboard = behavior.myBehaviorTree->GetBlackboard();

		TranslationComponent& translation = myWorld.GetComponent<TranslationComponent>(agent);
		blackboard.myPosition = translation.myOrientation.GetPos();

		blackboard.myTargetPosition = event.myPosition;
		blackboard.myTargetPosition.y += 1.f;

		aiEvents.myEvents.RemoveNonCyclicAtIndex(0);

		behavior.myBehaviorTree->Restart();
	}
}