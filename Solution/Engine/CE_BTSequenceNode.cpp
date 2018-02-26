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

eBTActionState CE_BTSequenceNode::Init(CE_Blackboard& aBlackboard)
{
	myCurrentIndex = 0;
	return CE_BTNode::Init(aBlackboard);
}

eBTActionState CE_BTSequenceNode::Update(CE_Blackboard& aBlackboard, float aDelta)
{
	eBTActionState childState = UpdateChild(myChildren[myCurrentIndex], aBlackboard, aDelta);

	if (childState == eBTActionState::FINISHED)
	{
		++myCurrentIndex;
		if (myCurrentIndex >= myChildren.Size())
			return eBTActionState::FINISHED;

		return eBTActionState::IN_PROGRESS;
	}

	return childState;
}
