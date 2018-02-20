#include "stdafx.h"
#include "CE_NavMesh.h"


namespace CE_NavMesh_private
{
	const int locNumQuadsPerSide = 10;
	const int locQuadSize = 2;
}

CE_NavMesh::CE_NavMesh()
{
	myTriangles.Reserve(CE_NavMesh_private::locNumQuadsPerSide * CE_NavMesh_private::locNumQuadsPerSide);

	CE_NavVertex bottomLeft;
	bottomLeft.myPosition.y = 1.1f;

	CE_NavVertex bottomRight;
	bottomRight.myPosition.y = 1.1f;

	CE_NavVertex topLeft;
	topLeft.myPosition.y = 1.1f;

	CE_NavVertex topRight;
	topRight.myPosition.y = 1.1f;

	for (int i = 0; i < CE_NavMesh_private::locNumQuadsPerSide; ++i)
	{
		for (int j = 0; j < CE_NavMesh_private::locNumQuadsPerSide; ++j)
		{
			bottomLeft.myPosition.x = static_cast<float>(j * CE_NavMesh_private::locQuadSize);
			bottomLeft.myPosition.z = static_cast<float>(i * CE_NavMesh_private::locQuadSize);

			bottomRight.myPosition.x = bottomLeft.myPosition.x + CE_NavMesh_private::locQuadSize;
			bottomRight.myPosition.z = bottomLeft.myPosition.z;

			topLeft.myPosition.x = bottomLeft.myPosition.x;
			topLeft.myPosition.z = bottomLeft.myPosition.z + CE_NavMesh_private::locQuadSize;

			topRight.myPosition.x = bottomRight.myPosition.x;
			topRight.myPosition.z = topLeft.myPosition.z;

			CreateTriangle(bottomLeft, topLeft, bottomRight);
			CreateTriangle(topLeft, topRight, bottomRight);
		}
	}
}


CE_NavMesh::~CE_NavMesh()
{
}

void CE_NavMesh::DebugDraw()
{
	for (const CE_NavTriangle& triangle : myTriangles)
		DebugDrawTriangle(triangle);
}

void CE_NavMesh::CreateTriangle(const CE_NavVertex& aVertex1, const CE_NavVertex& aVertex2, const CE_NavVertex& aVertex3)
{
	CE_NavEdge edge1;
	edge1.myVertex1 = aVertex1;
	edge1.myVertex2 = aVertex2;

	CE_NavEdge edge2;
	edge2.myVertex1 = aVertex2;
	edge2.myVertex2 = aVertex3;

	CE_NavEdge edge3;
	edge3.myVertex1 = aVertex3;
	edge3.myVertex2 = aVertex1;

	CE_NavTriangle triangle;
	triangle.myEdge1 = edge1;
	triangle.myEdge2 = edge2;
	triangle.myEdge3 = edge3;

	myTriangles.Add(triangle);
}

void CE_NavMesh::DebugDrawTriangle(const CE_NavTriangle& aTriangle) const
{
	CE_DRAW_LINE(aTriangle.myEdge1.myVertex1.myPosition, aTriangle.myEdge1.myVertex2.myPosition);
	CE_DRAW_LINE(aTriangle.myEdge2.myVertex1.myPosition, aTriangle.myEdge2.myVertex2.myPosition);
	CE_DRAW_LINE(aTriangle.myEdge3.myVertex1.myPosition, aTriangle.myEdge3.myVertex2.myPosition);
}
