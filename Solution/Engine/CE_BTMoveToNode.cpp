#include "stdafx.h"
#include "CE_BTMoveToNode.h"
#include "CE_BlackBoard.h"

eBTActionState CE_BTMoveToNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	CE_Vector3f dir = aBlackboard.myTargetPosition - aBlackboard.myPosition;
	float distance = CE_Length(dir);
	if (distance <= 0.1f)
	{
		aBlackboard.myPosition = aBlackboard.myTargetPosition;
		return eBTActionState::FINISHED;
	}

	CE_Normalize(dir);

	aBlackboard.myPosition += dir * aBlackboard.mySpeed * aDelta;
	return eBTActionState::IN_PROGRESS;
}
