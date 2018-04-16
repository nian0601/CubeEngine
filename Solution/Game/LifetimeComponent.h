#pragma once

#include <CE_BaseComponent.h>

struct LifetimeComponent : public CE_BaseComponent
{
	LifetimeComponent()
		: myTimeToLive(0.f)
		, myElapsedTime(0.f)
	{}

	float myTimeToLive;
	float myElapsedTime;
};