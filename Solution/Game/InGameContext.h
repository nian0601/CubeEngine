#pragma once

#include "GameContext.h"

class CE_Blackboard;
class CE_NavMesh;
class CE_PathFinder;
class CE_World;

class CPY_PhysicsWorld;

class EntityFactory;

class InGameContext : public GameContext
{
public:
	InGameContext();
	~InGameContext();

	virtual void Init(CE_Engine& anEngine) override;
	virtual void Update(float aDelta) override;
	virtual void Render() override;

private:
	void InitWorld();
	void InitGrid();
	void InitWater();
	void InitStone();
	void InitStockpile();

	CE_World* myWorld;

	EntityFactory* myEntityFactory;

	CE_Blackboard* myGlobalBlackboard;
	CE_NavMesh* myNavMesh;
	CE_PathFinder* myPathFinder;
	CPY_PhysicsWorld* myPhysicsWorld;
};

