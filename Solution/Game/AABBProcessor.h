#pragma once

#include <CE_BaseProcessor.h>

class AABBProcessor : public CE_BaseProcessor
{
public:
	AABBProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};
