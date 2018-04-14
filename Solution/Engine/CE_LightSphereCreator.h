#pragma once
#include "CE_IcoSphereCreator.h"

class CE_LightSphereCreator : public CE_IcoSphereCreator
{
public:
	CE_GrowingArray<CE_Pos_Vert> myVertices;

protected:
	int AddVertex(const CE_Vector4f& aPosition) override;
	const CE_Vector4f& GetPosition(int aVertexIndex) const override;
};
