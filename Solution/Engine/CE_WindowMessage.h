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
		CHARACTER,
	};

	eType myType;

	unsigned __int64 myLParams;
	unsigned __int64 myWParams;

	struct
	{
		int myLow;
		int myHigh;
	} myLParts;

	struct
	{
		int myLow;
		int myHigh;
	} myWParts;
};
