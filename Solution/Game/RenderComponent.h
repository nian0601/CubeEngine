#pragma once

#include <CE_BaseComponent.h>
#include "Types.h"

struct RenderComponent : public CE_BaseComponent
{
	struct Entry
	{
		Entry()
			: myScale(1.f)
			, myMetalness(1.f)
			, myRoughness(1.f)
			, myType(ModelType::eType::INVALID)
		{}
		CE_Matrix44f myOffsetMatrix;
		CE_Vector4f myColor;
		CE_Vector3f myScale;
		float myMetalness;
		float myRoughness;
		int myModelID;
		ModelType::eType myType;
	};

	CE_GrowingArray<Entry> myEntries;
};