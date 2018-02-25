#pragma once

#include "GameContext.h"
#include <CE_Path.h>
class CE_World;
class CE_NavMesh;
class CE_PathFinder;
class CE_Input;
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
	CE_World* myTemplateWorld;

	EntityFactory* myEntityFactory;

	CE_NavMesh* myNavMesh;
	CE_PathFinder* myPathFinder;
	CE_Input* myInput;
	CE_Path myPath;
};

