#pragma once

#include <CE_BaseProcessor.h>

class PlacingProcessor : public CE_BaseProcessor
{
public:
	PlacingProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

