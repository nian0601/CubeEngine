#pragma once

#include "CE_Vector2.h"

class CE_Camera;
class CE_WindowHandler;
class CE_GPUContext;
class CE_Renderer;
class CE_RendererProxy;
class CE_Time;

class CE_Game;

class CE_Engine
{
public:
	CE_Engine(CE_Game* aGame);
	~CE_Engine();

	void Run();

	CE_Camera& GetCamera();
	CE_RendererProxy& GetRendererProxy();

	const CE_GPUContext& GetGPUContext() const;
	const CE_Vector2i& GetWindowSize() const;

private:
	CE_WindowHandler* myWindowHandler;
	CE_GPUContext* myGPUContext;

	CE_Game* myGame;
	CE_Time* myTime;

	CE_Camera* myCamera;
	CE_Renderer* myRenderer;
	CE_RendererProxy* myRendererProxy;
};

