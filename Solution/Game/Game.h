#pragma once

#include <CE_Game.h>

class CE_World;
class EntityFactory;

class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render(CE_RendererProxy& anRendererProxy) override;

private:
	void CreateGrid();

	CE_World* myWorld;
	CE_World* myTemplateWorld;

	EntityFactory* myEntityFactory;
};