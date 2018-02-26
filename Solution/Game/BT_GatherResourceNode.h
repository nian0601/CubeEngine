#pragma once

#include <CE_BTNode.h>

class BT_GatherResourceNode : public CE_BTNode
{
public:
	eBTActionState Init(CE_Blackboard& aBlackboard) override;
	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;

private:
	float myTimer;
};

