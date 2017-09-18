#pragma once

#include <CE_BaseComponent.h>
#include <CE_StaticArray.h>

#include "Types.h"

struct InventoryComponent : public CE_BaseComponent
{
	InventoryComponent()
	{
		for (int i = 0; i < static_cast<int>(eItemType::_COUNT); ++i)
			myItems[i] = 0;
	}

	CE_StaticArray<int, static_cast<int>(eItemType::_COUNT)> myItems;
};
