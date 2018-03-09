#include "stdafx.h"
#include "CE_Path.h"
#include "CE_NavMeshPrimitives.h"

void CE_Path::AddWaypoint(const CE_Vector3f& aPosition)
{
	myWaypoints.InsertFirst(CE_Waypoint(aPosition));
}

void CE_Path::AddTriangleWaypoint(const CE_NavTriangle* aNavTriangle)
{
	myTriangleWaypoints.Add(CE_Waypoint(aNavTriangle, aNavTriangle->myCenter));
}

CE_Waypoint* CE_Path::GetNextWaypoint()
{
	if (myNextWaypoint >= myWaypoints.Size())
		return nullptr;

	CE_Waypoint* waypoint = &myWaypoints[myNextWaypoint];
	++myNextWaypoint;
	return waypoint;
}

void CE_Path::Reset()
{
	myNextWaypoint = 0;
	myWaypoints.RemoveAll();
}

void CE_Path::DebugDraw()
{
	for(int i = 0; i < myTriangleWaypoints.Size(); ++i)
	{
		const CE_NavTriangle* triangle = myTriangleWaypoints[i].myTriangle;
		DebugDrawTriangle(triangle, 0.2f, CE_Vector4f(0.f, 0.f, 1.f, 1.f));

		if (i < myTriangleWaypoints.Size() - 1)
		{
			const CE_NavTriangle* nextTriangle = myTriangleWaypoints[i+1].myTriangle;
			const CE_NavEdge* edge = triangle->GetSharedEdge(nextTriangle);

			const CE_NavPortal* portal = edge->GetPortal(triangle);

			CE_Vector3f pos1 = portal->myLeft;
			CE_Vector3f pos2 = portal->myRight;

			pos1.y += 0.3f;
			pos2.y += 0.3f;

			CE_DRAW_LINE_TWO_COLOR(pos1, pos2, CE_Vector4f(1.f, 0.f, 0.f, 1.f), CE_Vector4f(0.f, 1.f, 0.f, 1.f));
		}
	}

	for (int i = 0; i < myWaypoints.Size() - 1; ++i)
	{
		CE_Vector3f pos1 = myWaypoints[i].myPosition;
		CE_Vector3f pos2 = myWaypoints[i+1].myPosition;

		pos1.y += 0.5f;
		pos2.y += 0.5f;

		CE_DRAW_LINE_COLOR(pos1, pos2, CE_Vector4f(1.f, 1.f, 0.f, 1.f));
	}
}

void CE_Path::DebugDrawTriangle(const CE_NavTriangle* aTriangle, float aHeight, const CE_Vector4f aColor) const
{
	if (!aTriangle)
		return;

	for (int i = 0; i < 3; ++i)
	{
		const CE_NavEdge* edge = aTriangle->myEdges[i];
		CE_Vector3f pos1 = edge->myVertex1->myPosition;
		CE_Vector3f pos2 = edge->myVertex2->myPosition;

		pos1.y += aHeight;
		pos2.y += aHeight;

		CE_DRAW_LINE_COLOR(pos1, pos2, aColor);
	}
}