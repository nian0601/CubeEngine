#pragma once

#include <CE_BaseComponent.h>

struct RenderComponent : public CE_BaseComponent
{
	CE_Vector4f myColor;
	CE_Vector3f myScale;
};