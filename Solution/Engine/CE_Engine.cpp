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

CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CE_ASSERT(FAILED(hr) == false, "Failed to CoInitializeEx");

	myDirectX = new CE_DirectX();
	CE_WindowManager::Create(*myDirectX);
	myMainWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 1280, 720 }, "Cube Engine");
	//CE_WindowManager::GetInstance()->CreateNewWindow({ 1280, 720 }, "Cube Engine 2");

	myGPUContext = new CE_GPUContext(*myDirectX);

	myRenderer = new CE_Renderer(*myGPUContext);
	myRendererProxy = new CE_RendererProxy(*myRenderer);

	myCamera = new CE_Camera(myMainWindow->GetWindowSize());

	myTime = new CE_Time();
	myInput = new CE_Input(myMainWindow->GetHWND(), GetModuleHandle(NULL), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myGame->Init(*this);
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myInput);
	CE_SAFE_DELETE(myTime);
	CE_SAFE_DELETE(myCamera);
	CE_SAFE_DELETE(myRendererProxy);
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

		CE_GrowingArray<CE_Window*> windows = windowManager->GetWindows();
		for (CE_Window* window : windows)
		{
			window->PrepareForRender();
			myGame->Render(*myRendererProxy);

			myRenderer->Render(*myCamera);
			window->FinishRender();
		}

		myRenderer->Clear();
	}
}

CE_Camera& CE_Engine::GetCamera()
{
	return *myCamera;
}

CE_RendererProxy& CE_Engine::GetRendererProxy()
{
	return *myRendererProxy;
}

CE_Input& CE_Engine::GetInput()
{
	return *myInput;
}

CE_GPUContext& CE_Engine::GetGPUContext()
{
	return *myGPUContext;
}

const CE_Vector2i& CE_Engine::GetWindowSize() const
{
	return myMainWindow->GetWindowSize();
}
