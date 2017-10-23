#pragma once

#include <CE_BTNode.h>

class CE_World;
class BT_FindStockpileNode : public CE_BTNode
{
public:
	BT_FindStockpileNode(CE_World& aWorld);

	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;

private:
	CE_World& myWorld;
};

