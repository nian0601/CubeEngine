#pragma once

#include <CE_BaseProcessor.h>

class AIEventProcessor : public CE_BaseProcessor
{
public:
	AIEventProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

