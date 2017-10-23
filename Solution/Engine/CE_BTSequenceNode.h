#pragma once

#include "CE_BTNode.h"

class CE_BTSequenceNode : public CE_BTNode
{
public:
	CE_BTSequenceNode();
	~CE_BTSequenceNode();

	eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) override;
	void Restart() override;

	void AddChild(CE_BTNode* aNode) { myChildren.Add(aNode); }

private:
	CE_GrowingArray<CE_BTNode*> myChildren;
	int myCurrentIndex;
};

