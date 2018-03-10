#include "stdafx.h"
#include "CE_Path.h"
#include "CE_NavMeshPrimitives.h"

void CE_Path::AddWaypoint(const CE_Vector3f& aPosition)
{
	myWaypoints.Add(CE_Waypoint(aPosition));
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
	myTriangleWaypoints.RemoveAll();
}

void CE_Path::DebugDraw(const CE_Vector3f& aCurrentPosition)
{
	for(int i = 0; i < myTriangleWaypoints.Size(); ++i)
		DebugDrawTriangle(myTriangleWaypoints[i].myTriangle, 0.2f, CE_Vector4f(0.f, 0.f, 1.f, 1.f));

	CE_DRAW_LINE_COLOR_HEIGHT(aCurrentPosition, myWaypoints[myNextWaypoint-1].myPosition, 0.5f, CE_Vector4f(1.f, 1.f, 0.f, 1.f));

	for (int i = myNextWaypoint; i < myWaypoints.Size() - 1; ++i)
		CE_DRAW_LINE_COLOR_HEIGHT(myWaypoints[i].myPosition, myWaypoints[i+1].myPosition, 0.5f, CE_Vector4f(1.f, 1.f, 0.f, 1.f));
}

void CE_Path::DebugDrawTriangle(const CE_NavTriangle* aTriangle, float aHeight, const CE_Vector4f aColor) const
{
	if (!aTriangle)
		return;

	for (int i = 0; i < 3; ++i)
	{
		const CE_NavEdge* edge = aTriangle->myEdges[i];
		CE_DRAW_LINE_COLOR_HEIGHT(edge->myVertex1->myPosition, edge->myVertex2->myPosition, aHeight, aColor);
	}
}