#include "stdafx.h"
#include "CE_Path.h"
#include "CE_NavMeshPrimitives.h"

void CE_Path::AddWaypoint(const CE_NavTriangle* aNavTriangle)
{
	if (myWaypoints.Size() > 0)
		CE_ASSERT(myWaypoints.GetLast().myTriangle != nullptr, "Tried to AddWayPoint after the final waypoint was added");

	myWaypoints.InsertFirst(CE_Waypoint(aNavTriangle, aNavTriangle->myCenter));
}

void CE_Path::AddFinalWaypoint(const CE_Vector3f& aPosition)
{
	myWaypoints.Add(CE_Waypoint(aPosition));
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
	for (const CE_Waypoint& waypoint : myWaypoints)
		DebugDrawTriangle(waypoint.myTriangle, 0.2f, CE_Vector4f(0.f, 0.f, 1.f, 1.f));
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
