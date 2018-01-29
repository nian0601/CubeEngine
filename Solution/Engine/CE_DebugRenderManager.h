#pragma once

#include "CE_DebugPrimitives.h"

class CE_DebugRenderManager
{
public:
	CE_DebugRenderManager();
	~CE_DebugRenderManager();

	CE_GrowingArray<CE_Line> myLines;
};

