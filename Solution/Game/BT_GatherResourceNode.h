#pragma once

#include <CE_BTNode.h>

class BT_GatherResourceNode : public CE_BTNode
{
public:
	BT_GatherResourceNode();

	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;
	void Restart() override;

private:
	float myTimer;
};

