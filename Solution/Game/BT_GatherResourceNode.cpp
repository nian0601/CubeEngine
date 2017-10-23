#include "stdafx.h"
#include "BT_GatherResourceNode.h"


BT_GatherResourceNode::BT_GatherResourceNode()
{
	Restart();
}


eBTActionState BT_GatherResourceNode::Update(CE_Blackboard& /*aBlackboard*/, float aDelta)
{
	myTimer += aDelta;
	if (myTimer >= 3.f)
		return FINISHED;

	return IN_PROGRESS;
}

void BT_GatherResourceNode::Restart()
{
	myTimer = 0.f;
}
