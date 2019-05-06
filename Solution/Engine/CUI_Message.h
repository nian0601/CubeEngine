#pragma once

#include "CE_Any.h"

struct CUI_DragMessage
{
	enum
	{
		DRAG_START,
		DRAG_END
	};
	int myType;

	CE_Any myData;
};

struct CUI_MouseMessage
{
	enum Type
	{
		MOUSE_DOWN,
		MOUSE_UP,
		MOUSE_MOVE,
		MOUSE_WHEEL,
	};

	enum Button
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
	};

	enum Modifier
	{
		SHIFT = 1,
		CTRL = 2,
		ALT = 4,
	};

	bool ShiftDown() const { return (myModifiers & SHIFT) > 0; }
	bool CTRLDown() const { return (myModifiers & CTRL) > 0; }
	bool ALTDown() const { return (myModifiers & ALT) > 0; }

	int myType;
	int myMouseButton;
	int myModifiers;
	CE_Vector2f myOldPosition;
	CE_Vector2f myNewPosition;
	float myMouseWheelDelta;
};