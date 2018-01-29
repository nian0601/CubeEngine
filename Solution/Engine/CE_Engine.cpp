#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_CubeShader.h"
#include "CE_GPUContext.h"
#include "CE_Game.h"
#include "CE_Renderer.h"
#include "CE_Camera.h"
#include "CE_RendererProxy.h"
#include "CE_Time.h"
#include "CE_Input.h"
#include "CE_WindowManager.h"
#include "CE_DirectX.h"
#include "CE_Window.h"
#include "CE_DebugRenderManager.h"

CE_DebugRenderManager* CE_Engine::myDebugRenderManager = nullptr;

CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CE_ASSERT(FAILED(hr) == false, "Failed to CoInitializeEx");

	myDirectX = new CE_DirectX();
	CE_WindowManager::Create(*myDirectX);
	myMainWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 1280, 720 }, "Cube Engine");

	myGPUContext = new CE_GPUContext(*myDirectX);

	myRenderer = new CE_Renderer(*myGPUContext);

	myCamera = new CE_Camera(myMainWindow->GetWindowSize());

	myTime = new CE_Time();
	myInput = new CE_Input(myMainWindow->GetHWND(), GetModuleHandle(NULL), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myDebugRenderManager = new CE_DebugRenderManager();

	myGame->Init(*this);
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myGame);
	CE_SAFE_DELETE(myDebugRenderManager);
	CE_SAFE_DELETE(myInput);
	CE_SAFE_DELETE(myTime);
	CE_SAFE_DELETE(myCamera);
	CE_SAFE_DELETE(myRenderer);
	CE_SAFE_DELETE(myGPUContext);
	CE_WindowManager::Destory();
}

void CE_Engine::Run()
{
	CE_WindowManager* windowManager = CE_WindowManager::GetInstance();
	while (windowManager->PumpEvent())
	{
		myTime->Update();
		myInput->Update();

		if (myInput->KeyDown(DIK_ESCAPE))
			return;

		myGame->Update(myTime->GetFrameTime());
		myCamera->Update();

		myGame->Render();

		CE_GrowingArray<CE_Window*> windows = windowManager->GetWindows();
		for (CE_Window* window : windows)
		{
			window->PrepareForRender();

			myRenderer->Render3D(*myCamera, window->GetRendererProxy());
			myRenderer->Render2D(window->GetOrthagonalProjection(), window->GetRendererProxy());
			myRenderer->RenderLines(*myCamera, myDebugRenderManager->myLines);
			window->FinishRender();
		}
	}
}

CE_Camera& CE_Engine::GetCamera()
{
	return *myCamera;
}

CE_RendererProxy& CE_Engine::GetRendererProxy()
{
	return myMainWindow->GetRendererProxy();
}

CE_Input& CE_Engine::GetInput()
{
	return *myInput;
}

CE_DebugRenderManager& CE_Engine::GetDebugRenderManager()
{
	return *myDebugRenderManager;
}

CE_GPUContext& CE_Engine::GetGPUContext()
{
	return *myGPUContext;
}