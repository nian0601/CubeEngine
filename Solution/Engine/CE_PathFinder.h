#pragma once


class CE_NavMesh;
class CE_Path;

struct CE_NavTriangle;
struct CE_NavPortal;

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
	~CE_PathFinder();

	bool FindPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, CE_Path& aPathOut);

private:
	bool RunAStar(const CE_NavTriangle* aStartTriangle, const CE_NavTriangle* aEndTriangle, CE_GrowingArray<const CE_NavTriangle*>& someTrianglesOut);
	void FunnelPath(const CE_Vector3f& aStart, const CE_Vector3f& aEnd, const CE_GrowingArray<const CE_NavTriangle*>& someTriangles, CE_Path& aPathOut);
	void StringPull(const CE_GrowingArray<const CE_NavPortal*>& somePortals, CE_Path& aPathOut);

	float EstimateCost(const CE_NavTriangle* aStart, const CE_NavTriangle* aGoal) const;
	CE_PathNode* GetNode(const CE_NavTriangle* aTriangle);
	CE_PathNode* GetNextFreeNode();
	void AllocateNewNodes(int aCount);
	const CE_NavPortal* GetPortal(const CE_NavTriangle* aFirst, const CE_NavTriangle* aSecond, bool aUseFirst) const;
	float TriArea2(const CE_Vector3f& aA, const CE_Vector3f& aB, const CE_Vector3f& aC) const;
	bool VEqual(const CE_Vector3f& aA, const CE_Vector3f& aB) const;

	CE_GrowingArray<CE_PathNode*> myPathNodes;
	int myPathNodeCount;
	const CE_NavMesh& myNavMesh;
	CE_NavPortal* myStartPortal;
	CE_NavPortal* myEndPortal;
};