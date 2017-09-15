#pragma once

#include <CE_Game.h>

class CE_World;
class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render(CE_RendererProxy& anRendererProxy) override;

private:
	CE_World* myWorld;
};