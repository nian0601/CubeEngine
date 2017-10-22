#pragma once
#include "CE_BTNode.h"

class CE_BTMoveToNode : public CE_BTNode
{
public:
	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;
};

