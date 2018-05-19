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
	enum
	{
		MOUSE_DOWN,
		MOUSE_UP,
		MOUSE_MOVE,
	};

	int myType;
	CE_Vector2f myOldPosition;
	CE_Vector2f myNewPosition;
};