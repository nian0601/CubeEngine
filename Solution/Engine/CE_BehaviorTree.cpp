#include "stdafx.h"
#include "CE_BehaviorTree.h"
#include "CE_BTNode.h"

CE_BehaviorTree::CE_BehaviorTree()
{
}


CE_BehaviorTree::~CE_BehaviorTree()
{
}

void CE_BehaviorTree::Update()
{
	myRootNode->Update();
}
