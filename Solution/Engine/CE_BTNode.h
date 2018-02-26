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

	virtual eBTActionState Init(CE_Blackboard& aBlackboard) 
	{
		aBlackboard; 
		return eBTActionState::IN_PROGRESS;
	}
	virtual eBTActionState Update(CE_Blackboard& aBlackboard, float aDelta) 
	{ 
		aBlackboard;
		aDelta; 
		return eBTActionState::FINISHED;
	}
	virtual void DeInit(CE_Blackboard& aBlackboard) { aBlackboard; }

protected:
	CE_BTNode() {};
};

