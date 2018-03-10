#include "stdafx.h"
#include "CE_PathFinder.h"
#include "CE_NavMesh.h"

#include "CE_Heap.h"
#include "CE_NavMeshPrimitives.h"
#include "CE_Path.h"

namespace CE_PathFinder_private
{
	struct CE_NavPortal
	{
		CE_Vector3f myLeft;
		CE_Vector3f myRight;
	};

	CE_NavPortal CreatePortal(const CE_NavTriangle* aStartTriangle, const CE_NavTriangle* aEndTriangle)
	{
		const CE_NavEdge* sharedEdge = aStartTriangle->GetSharedEdge(aEndTriangle);

		float aX = aStartTriangle->myCenter.x;
		float aY = aStartTriangle->myCenter.z;
		float bX = aEndTriangle->myCenter.x;
		float bY = aEndTriangle->myCenter.z;
		float cX = sharedEdge->myVertex1->myPosition.x;
		float cY = sharedEdge->myVertex1->myPosition.z;

		// if d > 0, then vertex1 is the left vertex
		CE_NavPortal portal;
		float d = (bX - aX) * (cY - aY) - (bY - aY) * (cX - aX);
		if (d > 0.f)
		{
			portal.myLeft = sharedEdge->myVertex1->myPosition;
			portal.myRight = sharedEdge->myVertex2->myPosition;
		}
		else if (d < 0.f)
		{
			portal.myLeft = sharedEdge->myVertex2->myPosition;
			portal.myRight = sharedEdge->myVertex1->myPosition;
		}
		else
		{
			CE_ASSERT_ALWAYS("This shouldnt happen!");
		}

		return portal;
	}

	float TriArea2(const CE_Vector3f& aA, const CE_Vector3f& aB, const CE_Vector3f& aC)
	{
		float ax = aB.x - aA.x;
		float ay = aB.z - aA.z;
		float bx = aC.x - aA.x;
		float by = aC.z - aA.z;

		return bx*ay - ax*by;
	}

	bool VEqual(const CE_Vector3f& aA, const CE_Vector3f& aB)
	{
		static float eq = 0.001f * 0.001f;
		return CE_Length2(aA - aB) < eq;
	}
}

CE_PathFinder::CE_PathFinder(const CE_NavMesh& aNavMesh)
	: myNavMesh(aNavMesh)
	, myPathNodeCount(0)
{
	AllocateNewNodes(64);
}

CE_PathFinder::~CE_PathFinder()
{
	myPathNodes.DeleteAll();
}

bool CE_PathFinder::FindPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, CE_Path& aPathOut)
{
	const CE_NavTriangle* startTriangle = myNavMesh.FindTriangle(aStart);
	const CE_NavTriangle* endTriangle = myNavMesh.FindTriangle(aEnd);

	if (!startTriangle || !endTriangle)
		return false;

	if (startTriangle == endTriangle)
	{
		aPathOut.AddWaypoint(aEnd);
		return true;
	}

	CE_GrowingArray<const CE_NavTriangle*> triangles;
	if (RunAStar(endTriangle, startTriangle, triangles))
	{
		aPathOut.Reset();
		for (const CE_NavTriangle* triangle : triangles)
			aPathOut.AddTriangleWaypoint(triangle);

		FunnelPath(aStart, aEnd, triangles, aPathOut);

		return true;
	}

	return false;
}

bool CE_PathFinder::RunAStar(const CE_NavTriangle* aStartTriangle, const CE_NavTriangle* aEndTriangle, CE_GrowingArray<const CE_NavTriangle*>& someTrianglesOut)
{
	myPathNodeCount = 0;
	CE_PathNode* startNode = GetNode(aStartTriangle);
	startNode->myCurrentCost = 0.f;

	CE_Heap<CE_PathNode*, CE_PointerComparer<CE_PathNode*>> heap;
	heap.Enqueue(startNode);

	while (!heap.IsEmpty())
	{
		CE_PathNode* currNode = heap.Dequeue();

		if (currNode->myTriangle == aEndTriangle)
		{
			const CE_PathNode* node = currNode;
			while (node)
			{
				someTrianglesOut.Add(node->myTriangle);
				node = node->myParent;
			}

			return true;
		}

		for (int i = 0; i < 3; ++i)
		{
			const CE_NavTriangle* triangle = currNode->myTriangle->GetOtherTriangle(i);
			if (!triangle)
				continue;

			float newCost = currNode->myCurrentCost + EstimateCost(currNode->myTriangle, triangle);
			CE_PathNode* nextNode = GetNode(triangle);

			if (nextNode->myIsClosed)
				continue;

			if (nextNode->myIsOpen && nextNode->myCurrentCost <= newCost)
				continue;

			nextNode->myParent = currNode;
			nextNode->myCurrentCost = newCost;
			nextNode->myCostToGoal = EstimateCost(triangle, aEndTriangle);
			nextNode->myTotalCost = nextNode->myCurrentCost + nextNode->myCostToGoal;

			if (nextNode->myIsOpen)
			{
				heap.Heapify();
			}
			else
			{
				nextNode->myIsOpen = true;
				heap.Enqueue(nextNode);
			}
		}

		currNode->myIsOpen = false;
		currNode->myIsClosed = true;
	}

	return false;
}

void CE_PathFinder::FunnelPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, const CE_GrowingArray<const CE_NavTriangle*>& someTriangles, CE_Path& aPathOut)
{
	//Create Portals
	CE_GrowingArray<CE_PathFinder_private::CE_NavPortal> portals;
	for (int i = 0; i < someTriangles.Size() - 1; ++i)
	{
		portals.Add(CE_PathFinder_private::CreatePortal(someTriangles[i], someTriangles[i + 1]));
	}

	CE_PathFinder_private::CE_NavPortal& endPortal = portals.Add();
	endPortal.myLeft = CE_Vector3f(aEnd.x, 0.f, aEnd.z);
	endPortal.myRight = CE_Vector3f(aEnd.x, 0.f, aEnd.z);


	// Do StringPulling
	CE_Vector3f portalApex = CE_Vector3f(aStart.x, 0.f, aStart.z);
	CE_Vector3f portalLeft = portalApex;
	CE_Vector3f portalRight = portalApex;

	int apexIndex = 0;
	int leftIndex = 0;
	int rightIndex = 0;

	for (int i = 0; i < portals.Size(); ++i)
	{
		const CE_Vector3f& left = portals[i].myLeft;
		const CE_Vector3f& right = portals[i].myRight;

		//update right vertex
		if (CE_PathFinder_private::TriArea2(portalApex, portalRight, right) <= 0.0f)
		{
			if (CE_PathFinder_private::VEqual(portalApex, portalRight) || CE_PathFinder_private::TriArea2(portalApex, portalLeft, right) > 0.0f)
			{
				// tighten the funnel
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				aPathOut.AddWaypoint(portalLeft);

				// make current left the new apex.
				portalApex = portalLeft;
				apexIndex = leftIndex;
				// reset portal
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				i = apexIndex;
				continue;
			}
		}

		// update left vertex
		if (CE_PathFinder_private::TriArea2(portalApex, portalLeft, left) >= 0.0f)
		{
			if (CE_PathFinder_private::VEqual(portalApex, portalLeft) || CE_PathFinder_private::TriArea2(portalApex, portalRight, left) < 0.0f)
			{
				// tighten the funnel
				portalLeft = left;
				leftIndex = i;
			}
			else
			{
				// Left over reft, insert right to path and restart scan from portal right point.

				aPathOut.AddWaypoint(portalRight);

				// make current right the new apex.
				portalApex = portalRight;
				apexIndex = rightIndex;
				// reset portal
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				i = apexIndex;
				continue;
			}
		}
	}
}

float CE_PathFinder::EstimateCost(const CE_NavTriangle* aStart, const CE_NavTriangle* aGoal) const
{
	float x = fabsf(aStart->myCenter.x - aGoal->myCenter.x);
	float z = fabsf(aStart->myCenter.z - aGoal->myCenter.z);

	return x + z;
}

CE_PathNode* CE_PathFinder::GetNode(const CE_NavTriangle* aTriangle)
{
	for (int i = 0; i < myPathNodeCount; ++i)
	{
		if (myPathNodes[i]->myTriangle == aTriangle)
			return myPathNodes[i];
	}

	CE_PathNode* node = GetNextFreeNode();
	node->myTriangle = aTriangle;
	return node;
}

CE_PathNode* CE_PathFinder::GetNextFreeNode()
{
	if (myPathNodeCount >= myPathNodes.Size())
		AllocateNewNodes(myPathNodeCount);

	CE_PathNode* node = myPathNodes[myPathNodeCount];
	node->Reset();
	++myPathNodeCount;
	return node;
}

void CE_PathFinder::AllocateNewNodes(int aCount)
{
	for (int i = 0; i < aCount; ++i)
		myPathNodes.Add(new CE_PathNode());
}
