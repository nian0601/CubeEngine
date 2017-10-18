#pragma once

#include <CE_BaseComponent.h>
#include <CE_Vector3.h>
#include <CE_GrowingArray.h>

#include "Types.h"

struct CreateEntitySingletonComponent : public CE_BaseComponent
{
	struct Entry
	{
		eEntityTypes myEntityType;
		CE_Vector3f myPosition;
	};

	CE_GrowingArray<Entry> myEntries;
};
