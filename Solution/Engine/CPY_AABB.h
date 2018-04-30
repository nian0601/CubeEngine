#pragma once

#include "CE_Vector3.h"

struct CPY_AABB
{
	CPY_AABB() {}

	CPY_AABB(const CE_Vector3f& aPosition, const CE_Vector3f& aExtents)
	{
		myExtents = aExtents;
		Move(aPosition);
	}
	
	void Move(const CE_Vector3f& aNewPosition)
	{
		myCenterPos = aNewPosition;

		myMinPos = myCenterPos - myExtents * 0.5f;
		myMaxPos = myCenterPos + myExtents * 0.5f;
	}

	CE_Vector3f myCenterPos;
	CE_Vector3f myExtents;
	CE_Vector3f myMinPos;
	CE_Vector3f myMaxPos;
};