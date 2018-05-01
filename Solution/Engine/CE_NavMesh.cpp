#include "stdafx.h"
#include "CE_NavMesh.h"
#include "CE_NavMeshPrimitives.h"

namespace CE_NavMesh_private
{
	const int locNumQuadsPerSide = 10;
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
	CE_NavVertex* bottomLeftVertex = new CE_NavVertex(CE_Vector3f(0.f, 0.f, 0.f));

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

const CE_NavTriangle* CE_NavMesh::FindTriangle(const CE_Vector3f& aPosition) const
{
	for (const CE_NavTriangle* triangle : myTriangles)
	{
		if (TriangleVsPointCollision(triangle, aPosition))
			return triangle;
	}
		
	return nullptr;
}

void CE_NavMesh::DebugDraw()
{
	for (const CE_NavTriangle* triangle : myTriangles)
		DebugDrawTriangle(*triangle);
}

void CE_NavMesh::DebugDrawTriangle(const CE_NavTriangle& aTriangle) const
{
	const CE_Vector4f color(0.3f, 0.3f, 0.9f, 1.f);
	for (int i = 0; i < 3; ++i)
	{
		const CE_NavEdge* edge = aTriangle.myEdges[i];
		CE_DRAW_LINE_COLOR_HEIGHT(edge->myVertex1->myPosition, edge->myVertex2->myPosition, 0.1f, color);
	}
}

void CE_NavMesh::CreateQuad(CE_NavEdge* aLeftEdge, CE_NavEdge* aBottomEdge, CE_NavEdge*& aRightEdgeOut, CE_NavEdge*& aTopEdgeOut)
{
	CE_NavVertex* bottomLeftVertex = aBottomEdge->myVertex1;
	CE_NavVertex* bottomRightVertex = aBottomEdge->myVertex2;
	CE_NavVertex* topLeftVertex = aLeftEdge->myVertex2;
	CE_NavVertex* topRightVertex = new CE_NavVertex(topLeftVertex->myPosition + CE_NavMesh_private::locMoveX);

	CE_NavEdge* rightEdge = new CE_NavEdge(bottomRightVertex, topRightVertex);
	CE_NavEdge* topEdge = new CE_NavEdge(topLeftVertex, topRightVertex);
	CE_NavEdge* middleEdge = new CE_NavEdge(bottomLeftVertex, topRightVertex);

	CE_NavTriangle* triangle1 = new CE_NavTriangle(aBottomEdge, rightEdge, middleEdge);
	CE_NavTriangle* triangle2 = new CE_NavTriangle(aLeftEdge, topEdge, middleEdge);

	myTriangles.Add(triangle1);
	myTriangles.Add(triangle2);

	aRightEdgeOut = rightEdge;
	aTopEdgeOut = topEdge;
}

bool CE_NavMesh::TriangleVsPointCollision(const CE_NavTriangle* aTriangle, const CE_Vector3f& aPosition) const
{
	const CE_Vector3f& p = aPosition;
	const CE_Vector3f p0(aTriangle->myEdges[0]->myVertex1->myPosition);
	const CE_Vector3f p1(aTriangle->myEdges[0]->myVertex2->myPosition);

	const CE_NavVertex* opposite = aTriangle->GetOppositeVertex(aTriangle->myEdges[0]);
	const CE_Vector3f p2(opposite->myPosition);

	float A = 0.5f * (-p1.z * p2.x + p0.z * (-p1.x + p2.x) + p0.x * (p1.z - p2.z) + p1.x * p2.z);
	float sign = A < 0 ? -1.f : 1.f;
	float s = (p0.z * p2.x - p0.x * p2.z + (p2.z - p0.z) * p.x + (p0.x - p2.x) * p.z) * sign;
	float t = (p0.x * p1.z - p0.z * p1.x + (p0.z - p1.z) * p.x + (p1.x - p0.x) * p.z) * sign;

	return s > 0 && t > 0 && (s + t) < 2 * A * sign;
}
