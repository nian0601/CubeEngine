#pragma once

#include <CE_BaseComponent.h>
#include <CE_CustomEntityTypes.h>
#include "Types.h"

struct CursorSingletonComponent : public CE_BaseComponent
{
	CE_Entity myHoveredEntity;
};
