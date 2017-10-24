#pragma once

class CE_Blackboard;
class CE_BTNode;
class CE_BTInitNode;
class CE_BehaviorTree
{
public:
	CE_BehaviorTree();
	~CE_BehaviorTree();

	void Update(float aDelta);

	CE_Blackboard& GetBlackboard() { return *myBlackboard; }

	CE_BTInitNode& GetInitNode();

	bool IsFinished();
	void Restart();
	void Start();

private:
	CE_BTNode* myRootNode;
	bool myIsFinished;
	bool myIsStarted;

	CE_Blackboard* myBlackboard;
};
