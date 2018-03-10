#include "stdafx.h"
#include "CE_BTMoveToNode.h"
#include "CE_BlackBoard.h"
#include "CE_PathFinder.h"

eBTActionState CE_BTMoveToNode::Init(CE_Blackboard& aBlackboard)
{
	CE_Blackboard* globalBlackboard;
	if (!aBlackboard.Get("globalblackboard", globalBlackboard))
		return eBTActionState::FAILED;

	CE_PathFinder* pathFinder;
	if (!globalBlackboard->Get("pathfinder", pathFinder))
		return eBTActionState::FAILED;

	CE_Vector3f selfPosition;
	CE_Vector3f targetPosition;
	if (!aBlackboard.Get("selfPosition", selfPosition) || !aBlackboard.Get("targetPosition", targetPosition))
		return eBTActionState::FAILED;

	if (!pathFinder->FindPath(selfPosition, targetPosition, myPath))
		return eBTActionState::FAILED;

	myCurrentWaypoint = myPath.GetNextWaypoint();

	return eBTActionState::IN_PROGRESS;
}

eBTActionState CE_BTMoveToNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	CE_Vector3f selfPosition;
	float speed = 0.f;

	if (!aBlackboard.Get("selfPosition", selfPosition) || !aBlackboard.Get("speed", speed))
		return eBTActionState::FAILED;

	CE_Vector3f targetPosition = myCurrentWaypoint->myPosition;
	CE_Vector3f dir = targetPosition - selfPosition;
	float distance = CE_Length(dir);
	if (distance <= 0.1f)
	{
		myCurrentWaypoint = myPath.GetNextWaypoint();
		if (!myCurrentWaypoint)
		{
			aBlackboard.Set("selfPosition", targetPosition);
			return eBTActionState::FINISHED;
		}

		targetPosition = myCurrentWaypoint->myPosition;
		dir = targetPosition - selfPosition;
	}

	CE_Normalize(dir);

	selfPosition += dir * speed * aDelta;
	aBlackboard.Set("selfPosition", selfPosition);

	myPath.DebugDraw(selfPosition);
	return eBTActionState::IN_PROGRESS;
}
