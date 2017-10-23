#pragma once
#include "CE_BTNode.h"

class CE_BTInitNode : public CE_BTNode
{
public:
	CE_BTInitNode();
	~CE_BTInitNode();

	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;
	void Restart() override;

	void SetChildNode(CE_BTNode* aNode);

private:
	CE_BTNode* myChildNode;
};

