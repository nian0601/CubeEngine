#include "stdafx.h"

#include "CE_Engine.h"
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
#include "CE_DeferredRenderer.h"

#include "CE_ShaderManager.h"

#include "CE_TypeRegistration.h"
#include "CE_MaterialManager.h"
#include "CE_ObjManager.h"
#include "CE_TextureManager.h"

CE_DebugRenderManager* CE_Engine::myDebugRenderManager = nullptr;
CE_GPUContext* CE_Engine::myGPUContext = nullptr;
CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	CE_TypeRegistration::Register();

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CE_ASSERT(FAILED(hr) == false, "Failed to CoInitializeEx");

	myDirectX = new CE_DirectX();
	myGPUContext = new CE_GPUContext(*myDirectX);
	CE_ShaderManager::Create(*myGPUContext);
	CE_MaterialManager::Create();
	CE_ObjManager::Create();
	CE_TextureManager::Create(*myGPUContext);

	CE_WindowManager::Create(*myGPUContext);
	myMainWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 1920, 1080 }, "Cube Engine");

	myRenderer = new CE_Renderer(*myGPUContext);

	myTime = new CE_Time();
	myInput = new CE_Input(myMainWindow->GetHWND(), GetModuleHandle(NULL), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myDebugRenderManager = new CE_DebugRenderManager();

	myGame->Init(*this);
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myDebugRenderManager);
	CE_SAFE_DELETE(myInput);
	CE_SAFE_DELETE(myTime);
	CE_SAFE_DELETE(myRenderer);
	CE_ObjManager::Destroy();
	CE_MaterialManager::Destroy();
	CE_ShaderManager::Destroy();
	CE_SAFE_DELETE(myGPUContext);
	CE_SAFE_DELETE(myDirectX);
	CE_WindowManager::Destory();
}

void CE_Engine::Run()
{
	CE_WindowManager* windowManager = CE_WindowManager::GetInstance();
	while (windowManager->PumpEvent())
	{
		CE_ShaderManager::GetInstance()->Update();
		myTime->Update();
		myInput->Update();

		if (myInput->KeyDown(DIK_ESCAPE))
			return;

		static bool superSpeed = false;
		if (myInput->KeyDown(DIK_F1))
			superSpeed = !superSpeed;

		float deltaTime = myTime->GetFrameTime();
		if (superSpeed)
			deltaTime *= 40.f;

		myGame->Update(deltaTime);
		UpdateDebugCamera();

		myGame->Render();

		CE_GrowingArray<CE_Window*> windows = windowManager->GetWindows();
		for (CE_Window* window : windows)
		{
			window->ProcessUI(*myInput);

			window->BeginRender(*myRenderer);
			window->Render(*myRenderer);

			if (window == myMainWindow)
			{
				myRenderer->RenderLines(myDebugRenderManager->myLines);
				myDebugRenderManager->myLines.RemoveAll();
			}

			window->EndRender();
		}
	}
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

CE_Window& CE_Engine::GetMainWindow()
{
	return *myMainWindow;
}

void CE_Engine::UpdateDebugCamera()
{
	float cameraSpeed = 1.f;
	float rotationSpeed = CE_PI / 4.f;
	if (myInput->KeyIsPressed(DIK_LSHIFT))
		cameraSpeed = 10.f;
	else if (myInput->KeyIsPressed(DIK_LCONTROL))
		cameraSpeed = 0.1f;

	cameraSpeed *= myTime->GetFrameTime();
	rotationSpeed *= myTime->GetFrameTime();

	CE_Camera* camera = myMainWindow->GetCamera();
	const CE_Matrix44f& view = camera->GetNotInvertedView();
	if (myInput->KeyIsPressed(DIK_W))
		camera->Move(view.GetForward() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_S))
		camera->Move(-view.GetForward() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_A))
		camera->Move(-view.GetRight() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_D))
		camera->Move(view.GetRight() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_Q))
		camera->Move(-view.GetUp() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_E))
		camera->Move(view.GetUp() * cameraSpeed);

	if (myInput->KeyIsPressed(DIK_UP))
		camera->Rotate(CE_Matrix44f::CreateRotateAroundX(-rotationSpeed));
	if (myInput->KeyIsPressed(DIK_DOWN))
		camera->Rotate(CE_Matrix44f::CreateRotateAroundX(rotationSpeed));
	if (myInput->KeyIsPressed(DIK_LEFT))
		camera->Rotate(CE_Matrix44f::CreateRotateAroundY(-rotationSpeed));
	if (myInput->KeyIsPressed(DIK_RIGHT))
		camera->Rotate(CE_Matrix44f::CreateRotateAroundY(rotationSpeed));
}