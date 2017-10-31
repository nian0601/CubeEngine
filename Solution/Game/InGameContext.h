#pragma once

#include "GameContext.h"

class CE_World;

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
};
