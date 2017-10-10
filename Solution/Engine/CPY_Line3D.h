#pragma once

#include "CE_Vector3.h"

struct CPY_Line3D
{
	CPY_Line3D(){}
	CPY_Line3D(const CE_Vector3f& aStart, const CE_Vector3f& aEnd)
		: myStart(aStart)
		, myEnd(aEnd)
	{
	}

	CE_Vector3f myStart;
	CE_Vector3f myEnd;
};