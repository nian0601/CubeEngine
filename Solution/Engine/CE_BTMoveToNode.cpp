#include "stdafx.h"
#include "CE_BTMoveToNode.h"
#include "CE_BlackBoard.h"

eBTActionState CE_BTMoveToNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	CE_Vector3f selfPosition;
	CE_Vector3f targetPosition;
	float speed = 0.f;

	if (!aBlackboard.Get("selfPosition", selfPosition) || !aBlackboard.Get("targetPosition", targetPosition) || !aBlackboard.Get("speed", speed))
		return eBTActionState::FAILED;

	CE_Vector3f dir = targetPosition - selfPosition;
	float distance = CE_Length(dir);
	if (distance <= 0.1f)
	{
		aBlackboard.Set("selfPosition", targetPosition);
		return eBTActionState::FINISHED;
	}

	CE_Normalize(dir);

	selfPosition += dir * speed * aDelta;
	aBlackboard.Set("selfPosition", selfPosition);
	return eBTActionState::IN_PROGRESS;
}
