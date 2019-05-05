#pragma once

#include "GameContext.h"

class CE_World;
class EntityFactory;

class LightingTestContext : public GameContext
{
public:
	LightingTestContext();
	~LightingTestContext();

	virtual void Init(CE_Engine& anEngine) override;
	virtual void Update(float aDelta) override;
	virtual void Render() override;

private:
	void InitWorld();

	CE_World* myWorld;

	EntityFactory* myEntityFactory;

	CE_Entity myPointLight;
	CE_Entity myPointLight1;
	CE_Entity myPointLight2;
};

