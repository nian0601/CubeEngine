#pragma once

#include <CE_BaseProcessor.h>

class CE_Blackboard;

class BehaviorProcessor : public CE_BaseProcessor
{
public:
	BehaviorProcessor(CE_World& aWorld, CE_Blackboard& aGlobalBlackboard);

	void EntityAdded(CE_Entity anEntity) override;
	void Update(float aDelta) override;

private:
	CE_Blackboard& myGlobalBlackboard;
};

