#include "stdafx.h"
#include "CE_BTInitNode.h"


CE_BTInitNode::CE_BTInitNode()
	: myChildNode(nullptr)
{
}


CE_BTInitNode::~CE_BTInitNode()
{
	CE_SAFE_DELETE(myChildNode);
}

eBTActionState CE_BTInitNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	if (!myChildNode)
		return eBTActionState::FINISHED;

	return UpdateChild(myChildNode, aBlackboard, aDelta);
}

void CE_BTInitNode::SetChildNode(CE_BTNode* aNode)
{
	CE_ASSERT(myChildNode == nullptr, "Tried to SetChildNode multiple times");
	myChildNode = aNode;
}
