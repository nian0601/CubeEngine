#pragma once

#include "GameContext.h"

class CE_World;
class EntityFactory;

class NeuralNetworkContext : public GameContext
{
public:
	NeuralNetworkContext();
	~NeuralNetworkContext();

	virtual void Init(CE_Engine& anEngine) override;
	virtual void Update(float aDelta) override;
	virtual void Render() override;

	CE_World* myWorld;
	EntityFactory* myEntityFactory;
};

