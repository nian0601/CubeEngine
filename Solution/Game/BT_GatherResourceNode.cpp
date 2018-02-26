#include "stdafx.h"
#include "BT_GatherResourceNode.h"

eBTActionState BT_GatherResourceNode::Init(CE_Blackboard& aBlackboard)
{
	myTimer = 0.f;
	return CE_BTNode::Init(aBlackboard);
}

eBTActionState BT_GatherResourceNode::Update(CE_Blackboard& /*aBlackboard*/, float aDelta)
{
	myTimer += aDelta;
	if (myTimer >= 3.f)
		return eBTActionState::FINISHED;

	return eBTActionState::IN_PROGRESS;
}
