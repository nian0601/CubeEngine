#pragma once

enum eBTActionState
{
	IN_PROGRESS,
	FINISHED,
	FAILED,
};

class CE_Blackboard;
class CE_BTNode
{
public:
	virtual ~CE_BTNode() {};

	virtual eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) = 0;
	virtual void Restart() {};
protected:
	CE_BTNode() {};
};

