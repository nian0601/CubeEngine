#pragma once

#include <CE_BaseComponent.h>
#include <CE_StaticArray.h>
#include <CE_Assert.h>

enum eActions
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
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

	CE_StaticArray<int, static_cast<int>(_ACTIONCOUNT)> myKeyStates;
	CE_StaticArray<unsigned int, static_cast<int>(_ACTIONCOUNT)> myKeyMapping;

	const int myDownFlag = 1 << DOWN;
	const int myUpFlag = 1 << UP;
	const int myPressedFlag = 1 << PRESSED;
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
