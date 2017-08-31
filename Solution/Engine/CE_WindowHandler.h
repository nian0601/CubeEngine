#pragma once

#include <windows.h>

struct CE_WindowMessage
{
	enum eType
	{
		NONE,
		PAINT,
		DESTROY,
		ACTIVATE,
		SIZE,
		ENTER_SIZE_MOVE,
		EXIT_SIZE_MOVE,
	};

	eType myType;

	int myLowWordLParam;
	int myHighWordLParam;
	int myLowWordWParam;
	int myHighWordWParam;
};

class CE_WindowHandler
{
public:
	CE_WindowHandler(int aWidth, int aHeight);

	bool PumpEvent();

	void HandleWindowMessage(const CE_WindowMessage& aMessage);

	const CE_Vector2i& GetWindowSize() const { return myWindowSize; }
	const HWND& GetHWND() const { return myHWND; }

private:
	CE_Vector2i myWindowSize;
	HWND myHWND;
};

