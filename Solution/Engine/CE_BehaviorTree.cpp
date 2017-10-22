#include "stdafx.h"

#include "CE_BehaviorTree.h"
#include "CE_BTNode.h"
#include "CE_BTInitNode.h"
#include "CE_Blackboard.h"

CE_BehaviorTree::CE_BehaviorTree()
{
	myRootNode = new CE_BTInitNode();
	myIsFinished = false;

	myBlackboard = new CE_Blackboard();
}


CE_BehaviorTree::~CE_BehaviorTree()
{
	CE_SAFE_DELETE(myBlackboard);
	CE_SAFE_DELETE(myRootNode);
}

void CE_BehaviorTree::Update(float aDelta)
{
	if (myIsFinished)
		return;

	eBTActionState status = myRootNode->Update(*myBlackboard, aDelta);
	if (status != eBTActionState::IN_PROGRESS)
		myIsFinished = true;
}

CE_BTInitNode& CE_BehaviorTree::GetInitNode()
{
	return static_cast<CE_BTInitNode&>(*myRootNode);
}

bool CE_BehaviorTree::IsFinished()
{
	return myIsFinished;
}

void CE_BehaviorTree::Restart()
{
	myIsFinished = false;
}
