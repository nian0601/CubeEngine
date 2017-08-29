#pragma once

#include "CE_String.h"

#include <fstream>

class CE_DebugLogger
{
public:
	static void Destroy() { CE_SAFE_DELETE(ourInstance); };
	static CE_DebugLogger* GetInstance();

	void Assert(bool anExpression, char* aFile, char* aFunction, int aLine, const char* aFormattedString, ...);

private:
	CE_DebugLogger();
	~CE_DebugLogger();

	static CE_DebugLogger* ourInstance;

	std::ofstream myDebugFile;
	CE_String myTempString;
};

