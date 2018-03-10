#include "stdafx.h"
#include "CE_NavMeshPrimitives.h"

#define _USE_MATH_DEFINES
#include <math.h>

CE_NavVertex::CE_NavVertex(const CE_Vector3f& aPostion) 
	: myPosition(aPostion)
	, myRefCount(0)
{
}

CE_NavVertex::~CE_NavVertex()
{
	CE_ASSERT(myRefCount == 0, "NavVertex was deleted while still being referenced by someone!");
}

//////////////////////////////////////////////////////////////////////////

CE_NavEdge::CE_NavEdge(CE_NavVertex* aVertex1, CE_NavVertex* aVertex2)
	: myVertex1(aVertex1)
	, myVertex2(aVertex2)
	, myTriangle1(nullptr)
	, myTriangle2(nullptr)
{
	++myVertex1->myRefCount;
	++myVertex2->myRefCount;
}

CE_NavEdge::~CE_NavEdge()
{
	TryDeleteVertex(myVertex1);
	TryDeleteVertex(myVertex2);
}

void CE_NavEdge::TryDeleteVertex(CE_NavVertex* aVertex)
{
	--aVertex->myRefCount;
	if (aVertex->myRefCount == 0)
		CE_SAFE_DELETE(aVertex);
}

void CE_NavEdge::AddTriangle(const CE_NavTriangle* aTriangle)
{
	if (!myTriangle1)
	{
		myTriangle1 = aTriangle;
		return;
	}

	CE_ASSERT(!myTriangle2, "Tried to add more than two NavTriangles to an NavEdge!");
	myTriangle2 = aTriangle;
	CE_ASSERT(myTriangle1 != myTriangle2, "NavEdge storing same triangle twice");
}

void CE_NavEdge::RemoveTriangle(const CE_NavTriangle* aTriangle)
{
	if (myTriangle1 == aTriangle)
		myTriangle1 = nullptr;

	if (myTriangle2 == aTriangle)
		myTriangle2 = nullptr;
}

//////////////////////////////////////////////////////////////////////////

CE_NavTriangle::CE_NavTriangle(CE_NavEdge* aEdge1, CE_NavEdge* aEdge2, CE_NavEdge* aEdge3) 
{
	myEdges[0] = aEdge1;
	myEdges[1] = aEdge2;
	myEdges[2] = aEdge3;

	myCenter = myEdges[0]->myVertex1->myPosition;
	myCenter += myEdges[1]->myVertex2->myPosition;
	myCenter += GetOppositeVertex(myEdges[2])->myPosition;
	myCenter /= 3.f;

	for (int i = 0; i < 3; ++i)
		myEdges[i]->AddTriangle(this);
}

CE_NavTriangle::~CE_NavTriangle()
{
	for(int i = 0; i < 3; ++i)
		TryDeleteEdge(myEdges[i]);
}

void CE_NavTriangle::TryDeleteEdge(CE_NavEdge* aEdge)
{
	aEdge->RemoveTriangle(this);

	if (!aEdge->myTriangle1 && !aEdge->myTriangle2)
		CE_SAFE_DELETE(aEdge);
}

const CE_NavVertex* CE_NavTriangle::GetOppositeVertex(const CE_NavEdge* anEdge) const
{
	if (anEdge == myEdges[0])
		return GetSharedVertex(myEdges[1], myEdges[2]);

	if (anEdge == myEdges[1])
		return GetSharedVertex(myEdges[0], myEdges[2]);

	CE_ASSERT(anEdge == myEdges[2], "Tried to GetOppositeVertex from an edge thats not part of the triangle");
	return GetSharedVertex(myEdges[0], myEdges[1]);
}

const CE_NavVertex* CE_NavTriangle::GetSharedVertex(const CE_NavEdge* anEdge1, const CE_NavEdge* anEdge2) const
{
	if (anEdge1->myVertex1 == anEdge2->myVertex1 || anEdge1->myVertex1 == anEdge2->myVertex2)
		return anEdge1->myVertex1;

	CE_ASSERT(anEdge1->myVertex2 == anEdge2->myVertex1 || anEdge1->myVertex2 == anEdge2->myVertex2, "Failed to find SharedVertex");
	return anEdge1->myVertex2;
}

const CE_NavTriangle* CE_NavTriangle::GetOtherTriangle(int aIndex) const
{
	CE_ASSERT(aIndex >= 0 && aIndex < 3, "Invalid Index in GetOtherTriangle");

	const CE_NavEdge* edge = myEdges[aIndex];
	if (edge->myTriangle1 == this)
	{
		return edge->myTriangle2;
	}

	CE_ASSERT(edge->myTriangle2 == this, "Edge not part of triangle");
	return edge->myTriangle1;
}

const CE_NavEdge* CE_NavTriangle::GetSharedEdge(const CE_NavTriangle* aTriangle) const
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (myEdges[i] == aTriangle->myEdges[j])
				return myEdges[i];
		}
	}

	CE_ASSERT_ALWAYS("No Shared edge between triangles");
	return nullptr;
}
