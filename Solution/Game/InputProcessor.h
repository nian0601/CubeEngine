#pragma once

#include <CE_BaseProcessor.h>

class CE_Input;
class InputProcessor : public CE_BaseProcessor
{
public:
	InputProcessor(CE_World& aWorld, CE_Input& anInput);
	~InputProcessor();

	void Update(float aDelta) override;

private:
	CE_Input& myInput;
};

