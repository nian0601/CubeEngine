#pragma once
#include "CE_BTNode.h"
#include "CE_Path.h"

class CE_BTMoveToNode : public CE_BTNode
{
public:
	eBTActionState Init(CE_Blackboard& aBlackboard);
	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;

private:
	CE_Path myPath;
	CE_Waypoint* myCurrentWaypoint;
};

