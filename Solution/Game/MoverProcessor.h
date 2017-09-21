#pragma once

#include <CE_BaseProcessor.h>

class MoverProcessor : public CE_BaseProcessor
{
public:
	MoverProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

