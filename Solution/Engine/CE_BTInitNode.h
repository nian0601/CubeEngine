#pragma once
#include "CE_BTActionNode.h"

class CE_BTInitNode : public CE_BTActionNode
{
public:
	CE_BTInitNode();
	~CE_BTInitNode();

	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;

	void SetChildNode(CE_BTNode* aNode);

private:
	CE_BTNode* myChildNode;
};

