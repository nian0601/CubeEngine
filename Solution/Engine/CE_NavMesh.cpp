#include "stdafx.h"
#include "CE_NavMesh.h"


namespace CE_NavMesh_private
{
	const int locNumQuadsPerSide = 1;
	const float locQuadSize = 2.f;

	const CE_Vector3f locMoveX = CE_Vector3f(locQuadSize, 0.f, 0.f);
	const CE_Vector3f locMoveZ = CE_Vector3f(0.f, 0.f, locQuadSize);
}

CE_NavMesh::CE_NavMesh()
{
	CE_StaticArray<CE_NavEdge*, CE_NavMesh_private::locNumQuadsPerSide> prevTopEdges;
	for (int i = 0; i < CE_NavMesh_private::locNumQuadsPerSide; ++i)
		prevTopEdges[i] = nullptr;

	CE_NavEdge* prevRightEdge = nullptr;
	CE_NavVertex* bottomLeftVertex = new CE_NavVertex(CE_Vector3f(0.f, 1.1f, 0.f));

	for (int i = 0; i < CE_NavMesh_private::locNumQuadsPerSide; ++i)
	{
		CE_NavVertex* topLeftVertex = new CE_NavVertex(bottomLeftVertex->myPosition + CE_NavMesh_private::locMoveZ);
		CE_NavEdge* leftEdge = new CE_NavEdge(bottomLeftVertex, topLeftVertex);

		for (int j = 0; j < CE_NavMesh_private::locNumQuadsPerSide; ++j)
		{
			CE_NavEdge* bottomEdge = prevTopEdges[j];
			if (!bottomEdge)
			{
				CE_NavVertex* botRightVertex = new CE_NavVertex(leftEdge->myVertex1->myPosition + CE_NavMesh_private::locMoveX);
				bottomEdge = new CE_NavEdge(leftEdge->myVertex1, botRightVertex);
			}

			CreateQuad(leftEdge, bottomEdge, prevRightEdge, prevTopEdges[j]);

			leftEdge = prevRightEdge;
		}

		bottomLeftVertex = prevTopEdges[0]->myVertex1;
	}
}


CE_NavMesh::~CE_NavMesh()
{
	myTriangles.DeleteAll();
}

void CE_NavMesh::DebugDraw()
{
	for (const CE_NavTriangle* triangle : myTriangles)
		DebugDrawTriangle(*triangle);
}

void CE_NavMesh::DebugDrawTriangle(const CE_NavTriangle& aTriangle) const
{
	CE_DRAW_LINE(aTriangle.myEdge1->myVertex1->myPosition, aTriangle.myEdge1->myVertex2->myPosition);
	CE_DRAW_LINE(aTriangle.myEdge2->myVertex1->myPosition, aTriangle.myEdge2->myVertex2->myPosition);
	CE_DRAW_LINE(aTriangle.myEdge3->myVertex1->myPosition, aTriangle.myEdge3->myVertex2->myPosition);
}

void CE_NavMesh::CreateQuad(CE_NavEdge* aLeftEdge, CE_NavEdge* aBottomEdge, CE_NavEdge*& aRightEdgeOut, CE_NavEdge*& aTopEdgeOut)
{
	CE_NavVertex* topLeftVertex = aLeftEdge->myVertex2;
	CE_NavVertex* bottomRightVertex = aBottomEdge->myVertex2;
	CE_NavVertex* topRightVertex = new CE_NavVertex(topLeftVertex->myPosition + CE_NavMesh_private::locMoveX);

	CE_NavEdge* rightEdge = new CE_NavEdge(bottomRightVertex, topRightVertex);
	CE_NavEdge* topEdge = new CE_NavEdge(topLeftVertex, topRightVertex);
	CE_NavEdge* middleEdge = new CE_NavEdge(topLeftVertex, bottomRightVertex);

	CE_NavTriangle* triangle1 = new CE_NavTriangle(aLeftEdge, middleEdge, aBottomEdge);
	CE_NavTriangle* triangle2 = new CE_NavTriangle(topEdge, rightEdge, middleEdge);

	myTriangles.Add(triangle1);
	myTriangles.Add(triangle2);

	aRightEdgeOut = rightEdge;
	aTopEdgeOut = topEdge;
}