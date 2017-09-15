#pragma once

#include <CE_Game.h>
#include <CE_Matrix44.h>

class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update() override;
	void Render(CE_RendererProxy& anRendererProxy) override;

private:
	CE_Matrix44f myWorldMatrix;
};