#pragma once

#include <CE_BaseProcessor.h>

class CE_Input;
class MovementProcessor : public CE_BaseProcessor
{
public:
	MovementProcessor(CE_World& aWorld);

	void Update(float aDelta) override;
};

