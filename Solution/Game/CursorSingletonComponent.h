#pragma once

#include <CE_BaseComponent.h>
#include <CE_CustomEntityTypes.h>
#include "Types.h"

struct CursorSingletonComponent : public CE_BaseComponent
{
	CursorSingletonComponent()
		: myHoveredEntity(CE_Invalid_Entity)
	{}
	CE_Entity myHoveredEntity;
};
