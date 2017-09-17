#pragma once

#include <CE_BaseProcessor.h>

class CollisionProcessor : public CE_BaseProcessor
{
public:
	CollisionProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

