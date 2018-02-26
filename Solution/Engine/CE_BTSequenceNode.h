#pragma once

#include "CE_BTActionNode.h"

class CE_BTSequenceNode : public CE_BTActionNode
{
public:
	CE_BTSequenceNode();
	~CE_BTSequenceNode();

	eBTActionState Init(CE_Blackboard& aBlackboard);
	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;

	void AddChild(CE_BTNode* aNode) { myChildren.Add(aNode); }

private:
	CE_GrowingArray<CE_BTNode*> myChildren;
	int myCurrentIndex;
};

