#pragma once

#include "CE_Vector2.h"

class CE_WindowHandler;
class CE_GPUContext;

class CE_Game;

class CE_Engine
{
public:
	CE_Engine(CE_Game* aGame);
	~CE_Engine();

	void Run();

	const CE_GPUContext& GetGPUContext() const;
	const CE_Vector2i& GetWindowSize() const;

private:
	CE_WindowHandler* myWindowHandler;
	CE_GPUContext* myGPUContext;

	CE_Game* myGame;
};

