#include "stdafx.h"
#include "CE_ModelSphereCreator.h"

int CE_ModelSphereCreator::AddVertex(const CE_Vector3f& aPosition)
{
	CE_PosNormColor_Vert& vertex = myVertices.Add();
	vertex.myColor = CE_Vector4f(1.f, 1.f, 1.f, 1.f);
	vertex.myPosition = CE_GetNormalized(aPosition);
	vertex.myNormal = vertex.myPosition;

	return myVertices.Size() - 1;
}

const CE_Vector3f& CE_ModelSphereCreator::GetPosition(int aVertexIndex) const
{
	return myVertices[aVertexIndex].myPosition;
}
