#pragma once

struct CE_NavVertex
{
	CE_Vector3f myPosition;
};

struct CE_NavEdge
{
	CE_NavVertex myVertex1;
	CE_NavVertex myVertex2;
};

struct CE_NavTriangle
{
	CE_NavEdge myEdge1;
	CE_NavEdge myEdge2;
	CE_NavEdge myEdge3;
};

class CE_NavMesh
{
public:
	CE_NavMesh();
	~CE_NavMesh();

	void DebugDraw();

private:
	void CreateTriangle(const CE_NavVertex& aVertex1, const CE_NavVertex& aVertex2, const CE_NavVertex& aVertex3);
	void DebugDrawTriangle(const CE_NavTriangle& aTriangle) const;

	CE_GrowingArray<CE_NavTriangle> myTriangles;
};

