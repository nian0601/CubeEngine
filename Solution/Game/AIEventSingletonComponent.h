#pragma once

#include <CE_BaseComponent.h>
#include <CE_Vector3.h>
#include <CE_GrowingArray.h>

struct AIEventSingletonComponent : public CE_BaseComponent
{
	struct AIEvent
	{
		AIEvent() {}
		AIEvent(const CE_Vector3f& aPosition) : myPosition(aPosition) {}

		CE_Vector3f myPosition;
	};

	CE_GrowingArray<AIEvent> myEvents;
};
