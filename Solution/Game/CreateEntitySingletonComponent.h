#pragma once

#include <CE_BaseComponent.h>
#include <CE_Vector3.h>
#include <CE_GrowingArray.h>

struct CreateEntitySingletonComponent : public CE_BaseComponent
{
	struct Entry
	{
		int myEntityType;
		CE_Vector3f myPosition;
	};

	CE_GrowingArray<Entry> myEntries;
};
