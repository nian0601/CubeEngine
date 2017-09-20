#pragma once

#include <CE_BaseProcessor.h>

class CE_Input;
class MovementProcessor : public CE_BaseProcessor
{
public:
	MovementProcessor(CE_World& aWorld, CE_Input& aInput);

	void Update(float aDelta) override;

private:
	CE_Input& myInput;
};

