#pragma once

#include <CE_BaseProcessor.h>

class PickUpProcessor : public CE_BaseProcessor
{
public:
	PickUpProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

