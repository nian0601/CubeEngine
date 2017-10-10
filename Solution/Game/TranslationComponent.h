#pragma once

#include <CE_BaseComponent.h>

struct TranslationComponent : public CE_BaseComponent
{
	CE_Matrix44f myOrientation;
	CE_Vector3f myScale;
};