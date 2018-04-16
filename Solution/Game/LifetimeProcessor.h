#pragma once

#include <CE_BaseProcessor.h>

class LifetimeProcessor : public CE_BaseProcessor
{
public:
	LifetimeProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

