#pragma once

#include <CE_BaseComponent.h>
#include <CE_StaticArray.h>
#include <CE_Assert.h>

// Update constructor of InputProcessor to map an actual key to the action
enum eActions
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	PLACE_ENTITY,

	LBUTTON,
	RBUTTON,

	TOOL_BTN_1,
	TOOL_BTN_2,
	TOOL_BTN_3,
	TOOL_BTN_4,
	TOOL_BTN_5,

	_ACTIONCOUNT,
};

enum eActionState
{
	DOWN = 1,
	UP = 2,
	PRESSED = 3,
};

struct InputSingletonComponent : public CE_BaseComponent
{
	bool ActionDown(eActions anAction) const;
	bool ActionUp(eActions anAction) const;
	bool ActionIsPressed(eActions anAction) const;

	CE_Vector2f myMousePosition;

	CE_StaticArray<int, static_cast<int>(_ACTIONCOUNT)> myKeyStates;
	CE_StaticArray<unsigned int, static_cast<int>(_ACTIONCOUNT)> myKeyboardMapping;
	CE_StaticArray<unsigned int, static_cast<int>(_ACTIONCOUNT)> myMouseMappings;

	const int myDownFlag = 1 << DOWN;
	const int myUpFlag = 1 << UP;
	const int myPressedFlag = 1 << PRESSED;

	const unsigned int Invalid = static_cast<unsigned int>(-1);
};

inline bool InputSingletonComponent::ActionDown(eActions anAction) const
{
	CE_ASSERT(anAction >= 0 && anAction < _ACTIONCOUNT, "Invalid Action %i", anAction);

	return (myKeyStates[anAction] & myDownFlag) > 0;
}

inline bool InputSingletonComponent::ActionUp(eActions anAction) const
{
	CE_ASSERT(anAction >= 0 && anAction < _ACTIONCOUNT, "Invalid Action %i", anAction);

	return (myKeyStates[anAction] & myUpFlag) > 0;
}

inline bool InputSingletonComponent::ActionIsPressed(eActions anAction) const
{
	CE_ASSERT(anAction >= 0 && anAction < _ACTIONCOUNT, "Invalid Action %i", anAction);

	return (myKeyStates[anAction] & myPressedFlag) > 0;
}
