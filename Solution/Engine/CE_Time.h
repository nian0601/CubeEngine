#pragma once

typedef unsigned long long CE_TimeUnit;

class CE_Time
{
public:
	CE_Time();

	void Update();

	float GetFrameTime() const;
	CE_TimeUnit GetCurrent() const;

private:
	CE_TimeUnit myLastTime;
	CE_TimeUnit myCurrentTime;
	CE_TimeUnit myFrequency;
};

