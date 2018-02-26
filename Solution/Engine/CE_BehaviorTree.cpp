#include "stdafx.h"

#include "CE_BehaviorTree.h"
#include "CE_BTNode.h"
#include "CE_BTInitNode.h"
#include "CE_Blackboard.h"

CE_BehaviorTree::CE_BehaviorTree()
{
	myRootNode = new CE_BTInitNode();
	myIsRunning = false;

	myBlackboard = new CE_Blackboard();
}


CE_BehaviorTree::~CE_BehaviorTree()
{
	CE_SAFE_DELETE(myBlackboard);
	CE_SAFE_DELETE(myRootNode);
}

void CE_BehaviorTree::Update(float aDelta)
{
	if(!myIsRunning)
		return;

	eBTActionState status = myRootNode->Update(*myBlackboard, aDelta);
	if (status != eBTActionState::IN_PROGRESS)
	{
		myIsRunning = false;
	}
}

CE_BTInitNode& CE_BehaviorTree::GetInitNode()
{
	return static_cast<CE_BTInitNode&>(*myRootNode);
}

bool CE_BehaviorTree::IsRunning()
{
	return myIsRunning;
}

void CE_BehaviorTree::Restart()
{
	myIsRunning = true;
}