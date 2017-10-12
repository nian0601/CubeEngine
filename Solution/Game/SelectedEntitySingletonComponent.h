#pragma once

#include <CE_BaseComponent.h>
#include <CE_CustomEntityTypes.h>

struct SelectedEntitySingletonComponent : public CE_BaseComponent
{
	CE_Entity myHoveredEntity;
	CE_Entity mySelectedEntity;
};
