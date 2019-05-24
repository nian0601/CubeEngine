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
class CE_ShaderManager;
class CE_MaterialManager;
class CE_ObjManager;

class CE_Engine
{
public:
	CE_Engine(CE_Game* aGame);
	~CE_Engine();

	void Run();

	CE_RendererProxy& GetRendererProxy();
	CE_Input& GetInput();
	const CE_ObjManager& GetObjManager();

	CE_GPUContext& GetGPUContext();
	CE_Window& GetMainWindow();

	static CE_DebugRenderManager& GetDebugRenderManager();

private:
	void UpdateDebugCamera();
	CE_Window* myMainWindow;
	CE_DirectX* myDirectX;

	CE_GPUContext* myGPUContext;

	CE_Game* myGame;
	CE_Time* myTime;
	CE_Input* myInput;

	CE_ShaderManager* myShaderManager;
	CE_MaterialManager* myMaterialManager;
	CE_ObjManager* myObjManager;
	CE_Renderer* myRenderer;
	CE_DeferredRenderer* myDeferredRenderer;

	static CE_DebugRenderManager* myDebugRenderManager;
};

