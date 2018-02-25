#pragma once

struct CE_NavVertex
{
	CE_NavVertex(const CE_Vector3f& aPostion);
	~CE_NavVertex();

	CE_Vector3f myPosition;
	int myRefCount;
};

//////////////////////////////////////////////////////////////////////////

struct CE_NavTriangle;
struct CE_NavEdge
{
	CE_NavEdge(CE_NavVertex* aVertex1, CE_NavVertex* aVertex2);
	~CE_NavEdge();

	void TryDeleteVertex(CE_NavVertex* aVertex);

	void AddTriangle(const CE_NavTriangle* aTriangle);
	void RemoveTriangle(const CE_NavTriangle* aTriangle);

	CE_NavVertex* myVertex1;
	CE_NavVertex* myVertex2;

	const CE_NavTriangle* myTriangle1;
	const CE_NavTriangle* myTriangle2;
};

//////////////////////////////////////////////////////////////////////////

struct CE_NavTriangle
{
	CE_NavTriangle(CE_NavEdge* aEdge1, CE_NavEdge* aEdge2, CE_NavEdge* aEdge3);
	~CE_NavTriangle();

	void TryDeleteEdge(CE_NavEdge* aEdge);

	const CE_NavVertex* GetOppositeVertex(const CE_NavEdge* anEdge) const;
	const CE_NavVertex* GetSharedVertex(const CE_NavEdge* anEdge1, const CE_NavEdge* anEdge2) const;

	const CE_NavTriangle* GetOtherTriangle(int aIndex) const;

	CE_NavEdge* myEdges[3];
	CE_Vector3f myCenter;
};