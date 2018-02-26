#pragma once

struct CE_NavTriangle;
struct CE_Waypoint
{
	CE_Waypoint()
		: myTriangle(nullptr)
	{}

	CE_Waypoint(const CE_NavTriangle* aTriangle, const CE_Vector3f& aPosition)
		: myTriangle(aTriangle)
		, myPosition(aPosition)
	{}

	CE_Waypoint(const CE_Vector3f& aPosition)
		: myTriangle(nullptr)
		, myPosition(aPosition)
	{}
	const CE_NavTriangle* myTriangle;
	CE_Vector3f myPosition;
};

class CE_Path
{
public:
	void AddWaypoint(const CE_NavTriangle* aNavTriangle);
	void AddFinalWaypoint(const CE_Vector3f& aPosition);
	CE_Waypoint* GetNextWaypoint();

	void Reset();

	void DebugDraw();

private:
	void DebugDrawTriangle(const CE_NavTriangle* aTriangle, float aHeight, const CE_Vector4f aColor) const;

	CE_GrowingArray<CE_Waypoint> myWaypoints;
	int myNextWaypoint;
};

