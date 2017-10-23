#pragma once

#include <CE_BaseComponent.h>
#include "Types.h"

struct ResourceComponent : public CE_BaseComponent
{
	ResourceType::eType myResourceType;
};
