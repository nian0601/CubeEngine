#include "stdafx.h"
#include "CE_PathFinder.h"
#include "CE_NavMesh.h"

#include "CE_Heap.h"
#include "CE_NavMeshPrimitives.h"
#include "CE_Path.h"

CE_PathFinder::CE_PathFinder(const CE_NavMesh& aNavMesh)
	: myNavMesh(aNavMesh)
	, myPathNodeCount(0)
{
	AllocateNewNodes(64);
}

bool CE_PathFinder::FindPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, CE_Path& aPathOut)
{
	const CE_NavTriangle* startTriangle = myNavMesh.FindTriangle(aStart);
	const CE_NavTriangle* endTriangle = myNavMesh.FindTriangle(aEnd);

	if (!startTriangle || !endTriangle)
		return false;

	myPathNodeCount = 0;
	CE_PathNode* startNode = GetNode(startTriangle);
	startNode->myCurrentCost = 0.f;

	CE_Heap<CE_PathNode*, CE_PointerComparer<CE_PathNode*>> heap;
	heap.Enqueue(startNode);

	while (!heap.IsEmpty())
	{
		CE_PathNode* currNode = heap.Dequeue();

		if (currNode->myTriangle == endTriangle)
		{
			aPathOut.Reset();
			const CE_PathNode* node = currNode;
			while (node)
			{
				aPathOut.AddWaypoint(node->myTriangle);
				node = node->myParent;
			}

			aPathOut.AddFinalWaypoint(aEnd);
			
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
			nextNode->myCostToGoal = EstimateCost(triangle, endTriangle);
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

float CE_PathFinder::EstimateCost(const CE_NavTriangle* aStart, const CE_NavTriangle* aGoal) const
{
	float x = fabsf(aStart->myCenter.x - aGoal->myCenter.x);
	float z = fabsf(aStart->myCenter.z - aGoal->myCenter.z);

	return 10.f * (x + z);
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
	CE_PathNode* nodes = new CE_PathNode[aCount];
	for (int i = 0; i < aCount; ++i)
		myPathNodes.Add(&nodes[i]);
}