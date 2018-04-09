#pragma once

#include "CE_IcoSphereCreator.h"

class CE_ModelSphereCreator : public CE_IcoSphereCreator
{
public:
	CE_GrowingArray<CE_PosNormColor_Vert> myVertices;

protected:
	int AddVertex(const CE_Vector3f& aPosition) override;
	const CE_Vector3f& GetPosition(int aVertexIndex) const override;
};

