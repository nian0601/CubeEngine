#include "stdafx.h"
#include "CE_LightSphereCreator.h"

int CE_LightSphereCreator::AddVertex(const CE_Vector3f& aPosition)
{
	CE_Pos_Vert& vertex = myVertices.Add();
	vertex.myPosition = CE_GetNormalized(aPosition);

	return myVertices.Size() - 1;
}

const CE_Vector3f& CE_LightSphereCreator::GetPosition(int aVertexIndex) const
{
	return myVertices[aVertexIndex].myPosition;
}
