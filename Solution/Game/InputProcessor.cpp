#include "stdafx.h"

#include "InputProcessor.h"
#include "InputSingletonComponent.h"
#include "..\Engine\CE_Input.h"

InputProcessor::InputProcessor(CE_World& aWorld, CE_Input& anInput)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<>())
	, myInput(anInput)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	input.myKeyMapping[MOVE_FORWARD] = DIK_W;
	input.myKeyMapping[MOVE_BACKWARD] = DIK_S;
	input.myKeyMapping[MOVE_LEFT] = DIK_A;
	input.myKeyMapping[MOVE_RIGHT] = DIK_D;

	input.myKeyMapping[PLACE_ENTITY] = DIK_E;
}

void InputProcessor::Update(float /*aDelta*/)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	for (int i = 0; i < _ACTIONCOUNT; ++i)
	{
		input.myKeyStates[i] = 0;
		unsigned int realKey = input.myKeyMapping[i];

		if (myInput.KeyDown(realKey))
			input.myKeyStates[i] ^= input.myDownFlag;

		if (myInput.KeyUp(realKey))
			input.myKeyStates[i] ^= input.myUpFlag;

		if (myInput.KeyIsPressed(realKey))
			input.myKeyStates[i] ^= input.myPressedFlag;
	}
}