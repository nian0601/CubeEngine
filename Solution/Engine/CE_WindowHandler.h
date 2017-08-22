#pragma once

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
};

