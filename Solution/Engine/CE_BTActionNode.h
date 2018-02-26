#pragma once

#include "CE_BTNode.h"

class CE_BTActionNode : public CE_BTNode
{
public:
	CE_BTActionNode();

	eBTActionState Init(CE_Blackboard& aBlackboard) override;
	eBTActionState UpdateChild(CE_BTNode* aNode, CE_Blackboard& aBlackboard, float aDelta);

private:
	CE_BTNode* myUpdatingNode;
};

