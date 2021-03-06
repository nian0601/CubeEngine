#pragma once

class CE_Camera;
class CE_DebugRenderManager;
class CE_DeferredRenderer;
class CE_Input;
class CE_GPUContext;
class CE_Renderer;
class CE_RendererProxy;
class CE_Time;

class CE_Window;
class CE_DirectX;

class CE_Game;

class CE_Engine
{
public:
	CE_Engine(CE_Game* aGame);
	~CE_Engine();

	void Run();

	CE_RendererProxy& GetRendererProxy();
	CE_Input& GetInput();

	CE_Window& GetMainWindow();

	static CE_GPUContext& GetGPUContext();
	static CE_DebugRenderManager& GetDebugRenderManager();

private:
	void UpdateDebugCamera();
	CE_Window* myMainWindow;
	CE_DirectX* myDirectX;


	CE_Game* myGame;
	CE_Time* myTime;
	CE_Input* myInput;

	CE_Renderer* myRenderer;

	static CE_GPUContext* myGPUContext;
	static CE_DebugRenderManager* myDebugRenderManager;
};

