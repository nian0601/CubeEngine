#include "stdafx.h"
#include "CE_BTSequenceNode.h"


CE_BTSequenceNode::CE_BTSequenceNode()
	: myCurrentIndex(0)
{
}


CE_BTSequenceNode::~CE_BTSequenceNode()
{
	myChildren.DeleteAll();
}

eBTActionState CE_BTSequenceNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	eBTActionState childState = myChildren[myCurrentIndex]->Update(aBlackboard, aDelta);

	if (childState == FINISHED)
	{
		++myCurrentIndex;
		if (myCurrentIndex >= myChildren.Size())
			return FINISHED;

		return IN_PROGRESS;
	}

	return childState;
}

void CE_BTSequenceNode::Restart()
{
	for (CE_BTNode* child : myChildren)
		child->Restart();

	myCurrentIndex = 0;
}
