#include "stdafx.h"
#include "CE_BTActionNode.h"


CE_BTActionNode::CE_BTActionNode()
	: myUpdatingNode(nullptr)
{
}

eBTActionState CE_BTActionNode::Init(CE_Blackboard& /*aBlackboard*/)
{
	myUpdatingNode = nullptr;
	return eBTActionState::IN_PROGRESS;
}

eBTActionState CE_BTActionNode::UpdateChild(CE_BTNode* aNode, CE_Blackboard& aBlackboard, float aDelta)
{
	eBTActionState childStatus;
	if (myUpdatingNode == nullptr)
	{
		myUpdatingNode = aNode;
		childStatus = myUpdatingNode->Init(aBlackboard);
	}
	else
	{
		CE_ASSERT(myUpdatingNode == aNode, "Started updating another node before finishing the last one");
		childStatus = myUpdatingNode->Update(aBlackboard, aDelta);
	}

	if (childStatus == eBTActionState::FINISHED || childStatus == eBTActionState::FAILED)
	{
		myUpdatingNode->DeInit(aBlackboard);
		myUpdatingNode = nullptr;
	}

	return childStatus;
}
