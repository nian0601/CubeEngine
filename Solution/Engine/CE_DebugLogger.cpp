#include "stdafx.h"

#include "CE_DebugLogger.h"

#include <ctime>
#include <cstdarg>
#include <windows.h>
#include <iostream>

CE_DebugLogger* CE_DebugLogger::ourInstance = nullptr;

CE_DebugLogger* CE_DebugLogger::GetInstance()
{
	if (!ourInstance)
		ourInstance = new CE_DebugLogger();

	return ourInstance;
}

void CE_DebugLogger::Assert(bool anExpression, char* aFile, char* aFunction, int aLine, const char* aFormattedString, ...)
{
	if (anExpression)
		return;


	//Get the formated VA_ARGS string
	char localFormated[1024];
	strcpy_s(localFormated, aFormattedString);

	char buffer[1024];
	va_list args;
	va_start(args, aFormattedString);
	if (vsprintf_s(buffer, aFormattedString, args) < 0)
	{
		perror(buffer);
	}
	va_end(args);

	//Combine VA_ARGS string with general debug-output
	myTempString.Clear();
	myTempString += "!! ASSERTION !!\n";
	myTempString += "Error: ";
	myTempString += buffer;
	myTempString += "\nFile: ";
	myTempString += aFile;
	myTempString += "\nFunction: ";
	myTempString += aFunction;
	myTempString += " (Line: ";
	myTempString += aLine;
	myTempString += ")\n";

	myDebugFile << myTempString.c_str();
	
	std::cout << myTempString.c_str();
	int* assertPtr = nullptr;
	*assertPtr = 1;
}

CE_DebugLogger::CE_DebugLogger()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[30];
	localtime_s(&tstruct, &now);

	strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", &tstruct);

	CE_String logPath = "log\\";

	CreateDirectory("log", NULL);

	logPath += buf;
	logPath += "_DebugLogger.txt";
	myDebugFile.open(logPath.c_str());
}


CE_DebugLogger::~CE_DebugLogger()
{
	myDebugFile.close();
}
