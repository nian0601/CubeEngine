#include "stdafx.h"
#include "CE_Time.h"
#include <windows.h>


CE_Time::CE_Time()
{
	LARGE_INTEGER largeInt;
	QueryPerformanceFrequency(&largeInt);

	myFrequency = largeInt.QuadPart;

	QueryPerformanceCounter(&largeInt);
	myCurrentTime = largeInt.QuadPart * 1000000 / myFrequency;
	myLastTime = myCurrentTime;
}

void CE_Time::Update()
{
	myLastTime = myCurrentTime;

	LARGE_INTEGER largeInt;
	QueryPerformanceCounter(&largeInt);
	myCurrentTime = largeInt.QuadPart * 1000000 / myFrequency;
}

float CE_Time::GetFrameTime() const
{
	return static_cast<float>(myCurrentTime - myLastTime) / 1000000.f;
}

CE_TimeUnit CE_Time::GetCurrent() const
{
	return myCurrentTime;
}