#include "stdafx.h"

#include "CE_DebugLogger.h"


#include <ctime>
#include <cstdarg>
#include <windows.h>

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
	va_start(args, localFormated);
	vsprintf_s(buffer, localFormated, args);
	perror(buffer);
	va_end(args);

	//Combine VA_ARGS string with general debug-output
	myTempString.Clear();
	myTempString += "\nError:\n";
	myTempString += buffer;
	myTempString += "\n\nFunction: ";
	myTempString += aFunction;
	myTempString += "\n";

	myDebugFile << myTempString.c_str();

	//Convert assert-message into wchar
	const int messageTotalSize = strlen(myTempString.c_str()) + 1;
	wchar_t* wcharMessage = new wchar_t[messageTotalSize];
	size_t tempSize;
	mbstowcs_s(&tempSize, wcharMessage, messageTotalSize, myTempString.c_str(), messageTotalSize);

	//Convert file-name into wchar
	const int filenameTotalSize = strlen(aFile) + 1;
	wchar_t* wcharFile = new wchar_t[filenameTotalSize];
	mbstowcs_s(&tempSize, wcharFile, filenameTotalSize, aFile, filenameTotalSize);

	_wassert(wcharMessage, wcharFile, aLine);

	CE_SAFE_DELETE_ARRAY(wcharMessage);
	CE_SAFE_DELETE_ARRAY(wcharFile);

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
