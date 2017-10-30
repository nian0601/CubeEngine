#pragma once

#include <CE_BTNode.h>
class BT_FindStockpileNode : public CE_BTNode
{
public:
	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;
};

