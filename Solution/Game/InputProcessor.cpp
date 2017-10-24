#include "stdafx.h"

#include "InputProcessor.h"
#include "InputSingletonComponent.h"
#include "..\Engine\CE_Input.h"

InputProcessor::InputProcessor(CE_World& aWorld, CE_Input& anInput)
	: CE_BaseProcessor(aWorld)
	, myInput(anInput)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	for (int i = 0; i < _ACTIONCOUNT; ++i)
	{
		input.myKeyboardMapping[i] = input.Invalid;
		input.myMouseMappings[i] = input.Invalid;
	}

	input.myKeyboardMapping[MOVE_FORWARD] = DIK_W;
	input.myKeyboardMapping[MOVE_BACKWARD] = DIK_S;
	input.myKeyboardMapping[MOVE_LEFT] = DIK_A;
	input.myKeyboardMapping[MOVE_RIGHT] = DIK_D;

	input.myMouseMappings[LBUTTON] = 0;
	input.myMouseMappings[RBUTTON] = 1;
}

void InputProcessor::Update(float /*aDelta*/)
{
	InputSingletonComponent& input = myWorld.GetSingletonComponent<InputSingletonComponent>();

	input.myMousePosition = myInput.GetMousePosition();

	for (int i = 0; i < _ACTIONCOUNT; ++i)
	{
		input.myKeyStates[i] = 0;

		unsigned int realKey = input.myKeyboardMapping[i];
		if (realKey != input.Invalid)
		{
			if (myInput.KeyDown(realKey))
				input.myKeyStates[i] ^= input.myDownFlag;

			if (myInput.KeyUp(realKey))
				input.myKeyStates[i] ^= input.myUpFlag;

			if (myInput.KeyIsPressed(realKey))
				input.myKeyStates[i] ^= input.myPressedFlag;
		}
		else
		{
			realKey = input.myMouseMappings[i];
			CE_ASSERT(realKey != input.Invalid, "Unbound action!");

			if (myInput.MouseDown(realKey))
				input.myKeyStates[i] ^= input.myDownFlag;

			if (myInput.MouseUp(realKey))
				input.myKeyStates[i] ^= input.myUpFlag;

			if (myInput.MouseIsPressed(realKey))
				input.myKeyStates[i] ^= input.myPressedFlag;
		}
	}
}