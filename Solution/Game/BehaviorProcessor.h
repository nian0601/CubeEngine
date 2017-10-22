#pragma once

#include <CE_BaseProcessor.h>

class CE_Input;
class BehaviorProcessor : public CE_BaseProcessor
{
public:
	BehaviorProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

