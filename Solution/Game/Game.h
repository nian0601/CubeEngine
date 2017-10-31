#pragma once

#include <CE_Game.h>

class GameContext;

class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	GameContext* myGameContext;
};