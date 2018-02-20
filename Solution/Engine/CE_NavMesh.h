#pragma once

struct CE_NavVertex
{
	CE_NavVertex(const CE_Vector3f& aPostion)
		: myPosition(aPostion)
		, myRefCount(0)
	{}

	~CE_NavVertex()
	{
		CE_ASSERT(myRefCount == 0, "NavVertex was deleted while still being referenced by someone!");
	}

	CE_Vector3f myPosition;
	int myRefCount;
};

struct CE_NavTriangle;
struct CE_NavEdge
{
	CE_NavEdge(CE_NavVertex* aVertex1, CE_NavVertex* aVertex2)
		: myVertex1(aVertex1)
		, myVertex2(aVertex2)
		, myTriangle1(nullptr)
		, myTriangle2(nullptr)
	{
		++myVertex1->myRefCount;
		++myVertex2->myRefCount;
	}
	
	~CE_NavEdge()
	{
		TryDeleteVertex(myVertex1);
		TryDeleteVertex(myVertex2);
	}

	void TryDeleteVertex(CE_NavVertex* aVertex)
	{
		--aVertex->myRefCount;
		if (aVertex->myRefCount == 0)
			CE_SAFE_DELETE(aVertex);
	}

	void AddTriangle(const CE_NavTriangle* aTriangle)
	{
		if (!myTriangle1)
		{
			myTriangle1 = aTriangle;
			return;
		}

		CE_ASSERT(!myTriangle2, "Tried to add more than two NavTriangles to an NavEdge!");
		myTriangle2 = aTriangle;
	}

	void RemoveTriangle(const CE_NavTriangle* aTriangle)
	{
		if (myTriangle1 == aTriangle)
			myTriangle1 = nullptr;

		if (myTriangle2 == aTriangle)
			myTriangle2 = nullptr;
	}

	CE_NavVertex* myVertex1;
	CE_NavVertex* myVertex2;

	const CE_NavTriangle* myTriangle1;
	const CE_NavTriangle* myTriangle2;
};

struct CE_NavTriangle
{
	CE_NavTriangle(CE_NavEdge* aEdge1, CE_NavEdge* aEdge2, CE_NavEdge* aEdge3)
		: myEdge1(aEdge1)
		, myEdge2(aEdge2)
		, myEdge3(aEdge3)
	{
		myEdge1->AddTriangle(this);
		myEdge2->AddTriangle(this);
		myEdge3->AddTriangle(this);
	}

	~CE_NavTriangle()
	{
		TryDeleteEdge(myEdge1);
		TryDeleteEdge(myEdge2);
		TryDeleteEdge(myEdge3);
	}

	void TryDeleteEdge(CE_NavEdge* aEdge)
	{
		aEdge->RemoveTriangle(this);

		if (!aEdge->myTriangle1 && !aEdge->myTriangle2)
			CE_SAFE_DELETE(aEdge);
	}

	CE_NavEdge* myEdge1;
	CE_NavEdge* myEdge2;
	CE_NavEdge* myEdge3;
};

class CE_NavMesh
{
public:
	CE_NavMesh();
	~CE_NavMesh();

	void DebugDraw();

private:
	void DebugDrawTriangle(const CE_NavTriangle& aTriangle) const;

	void CreateQuad(CE_NavEdge* aLeftEdge, CE_NavEdge* aBottomEdge, CE_NavEdge*& aRightEdgeOut, CE_NavEdge*& aTopEdgeOut);

	CE_GrowingArray<CE_NavTriangle*> myTriangles;
};

