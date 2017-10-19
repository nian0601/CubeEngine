#pragma once

class CE_BTNode;
class CE_BehaviorTree
{
public:
	CE_BehaviorTree();
	~CE_BehaviorTree();

	void Update();

private:
	CE_BTNode* myRootNode;
};

