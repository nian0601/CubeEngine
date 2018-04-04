#pragma once

#include <CE_BaseComponent.h>

struct RotationComponent : public CE_BaseComponent
{
	RotationComponent()
		: mySpeeds(1.f)
	{}

	CE_Vector3f mySpeeds;
};