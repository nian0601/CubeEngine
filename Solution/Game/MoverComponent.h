#pragma once

#include <CE_BaseComponent.h>
#include <CE_Vector3.h>

struct MoverComponent : public CE_BaseComponent
{
	float mySpeed;
	CE_Vector3f myDirection;
};
