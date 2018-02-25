#pragma once


class CE_NavMesh;
class CE_Path;

struct CE_NavTriangle;
struct CE_PathNode
{
	CE_PathNode()
		: myTriangle(nullptr)
	{
		Reset();
	}

	bool operator< (const CE_PathNode& aOther) const
	{
		return myTotalCost < aOther.myTotalCost;
	}

	bool operator==(const CE_NavTriangle* aTriangle) const
	{
		return myTriangle == aTriangle;
	}

	void Reset()
	{
		myTriangle = nullptr;
		myParent = nullptr;
		myCostToGoal = 0.f;
		myCurrentCost = FLT_MAX;
		myTotalCost = myCurrentCost;
		myIsOpen = false;
		myIsClosed = false;
	}

	const CE_NavTriangle* myTriangle;
	CE_PathNode* myParent;

	float myCostToGoal;
	float myCurrentCost;
	float myTotalCost;

	bool myIsOpen;
	bool myIsClosed;
};

class CE_PathFinder
{
public:
	CE_PathFinder(const CE_NavMesh& aNavMesh);

	bool FindPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, CE_Path& aPathOut);

private:
	float EstimateCost(const CE_NavTriangle* aStart, const CE_NavTriangle* aGoal) const;
	CE_PathNode* GetNode(const CE_NavTriangle* aTriangle);
	CE_PathNode* GetNextFreeNode();
	void AllocateNewNodes(int aCount);

	CE_GrowingArray<CE_PathNode*> myPathNodes;
	int myPathNodeCount;
	const CE_NavMesh& myNavMesh;
};