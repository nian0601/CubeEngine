#pragma once

struct CE_NavTriangle;
struct CE_NavEdge;

class CE_NavMesh
{
public:
	CE_NavMesh();
	~CE_NavMesh();

	const CE_NavTriangle* FindTriangle(const CE_Vector3f& aPosition) const;
	void DebugDraw();

private:
	void DebugDrawTriangle(const CE_NavTriangle& aTriangle) const;

	void CreateQuad(CE_NavEdge* aLeftEdge, CE_NavEdge* aBottomEdge, CE_NavEdge*& aRightEdgeOut, CE_NavEdge*& aTopEdgeOut);

	bool TriangleVsPointCollision(const CE_NavTriangle* aTriangle, const CE_Vector3f& aPosition) const;

	CE_GrowingArray<CE_NavTriangle*> myTriangles;
};

