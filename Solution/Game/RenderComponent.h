#pragma once

#include <CE_BaseComponent.h>

struct RenderComponent : public CE_BaseComponent
{
	struct Entry
	{
		Entry()
			: myScale(1.f)
			, myMetalness(1.f)
			, myRoughness(1.f)
		{}
		CE_Matrix44f myOffsetMatrix;
		CE_Vector4f myColor;
		CE_Vector3f myScale;
		float myMetalness;
		float myRoughness;
	};

	CE_GrowingArray<Entry> myEntries;
};