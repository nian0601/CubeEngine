#pragma once

#include <CE_BaseProcessor.h>

class RotationProcessor : public CE_BaseProcessor
{
public:
	RotationProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

