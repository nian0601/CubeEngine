#pragma once

class CE_Camera;
class CE_Input;
class CE_GPUContext;
class CE_Renderer;
class CE_RendererProxy;
class CE_Time;

class CE_Window;
class CE_WindowManager;
class CE_DirectX;

class CE_Game;

class CE_Engine
{
public:
	CE_Engine(CE_Game* aGame);
	~CE_Engine();

	void Run();

	CE_Camera& GetCamera();
	CE_RendererProxy& GetRendererProxy();
	CE_Input& GetInput();

	CE_GPUContext& GetGPUContext();

private:
	CE_Window* myMainWindow;
	CE_DirectX* myDirectX;

	CE_GPUContext* myGPUContext;

	CE_Game* myGame;
	CE_Time* myTime;
	CE_Input* myInput;

	CE_Camera* myCamera;
	CE_Renderer* myRenderer;
};

