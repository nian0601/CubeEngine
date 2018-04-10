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

CE_DebugRenderManager* CE_Engine::myDebugRenderManager = nullptr;

CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CE_ASSERT(FAILED(hr) == false, "Failed to CoInitializeEx");

	myDirectX = new CE_DirectX();
	myGPUContext = new CE_GPUContext(*myDirectX);
	CE_WindowManager::Create(*myGPUContext);
	myMainWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 1280, 720 }, "Cube Engine");


	myRenderer = new CE_Renderer(*myGPUContext);
	myDeferredRenderer = new CE_DeferredRenderer(*myGPUContext, myMainWindow->GetWindowSize());

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
	CE_SAFE_DELETE(myDeferredRenderer);
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
		UpdateDebugCamera();
		myCamera->Update();

		myGame->Render();

		CE_GrowingArray<CE_Window*> windows = windowManager->GetWindows();
		for (CE_Window* window : windows)
		{
			window->PrepareForRender();

			myDeferredRenderer->SetBackbuffer(window->GetBackbuffer());
			myDeferredRenderer->Render(*myRenderer, *myCamera, window->GetRendererProxy());

			//myDeferredRenderer->BeginGBuffer(window->GetBackbuffer());
			//myRenderer->Render3D(*myCamera, window->GetRendererProxy());
			//myDeferredRenderer->EndGBuffer(window->GetBackbuffer());
			//
			//myDeferredRenderer->RenderToScreen(myCamera);
			//myDeferredRenderer->RenderPointLights(*myCamera, window->GetRendererProxy());
			
			// myRenderer->RenderLights();

			myRenderer->Render2D(window->GetOrthagonalProjection(), window->GetRendererProxy());
			myRenderer->RenderLines(*myCamera, myDebugRenderManager->myLines);
			myDebugRenderManager->myLines.RemoveAll();

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

	/*const CE_Vector3f right(1.f, 0.f, 0.f);
	const CE_Vector3f up(0.f, 1.f, 0.f);
	const CE_Vector3f forward(0.f, 0.f, 1.f);

	CE_Vector3f finalMove(0.f, 0.f, 0.f);
	if (myInput->KeyIsPressed(DIK_W))
		finalMove += forward;
	if (myInput->KeyIsPressed(DIK_S))
		finalMove -= forward;
	if (myInput->KeyIsPressed(DIK_A))
		finalMove -= right;
	if (myInput->KeyIsPressed(DIK_D))
		finalMove += right;
	if (myInput->KeyIsPressed(DIK_Q))
		finalMove -= up;
	if (myInput->KeyIsPressed(DIK_E))
		finalMove += up;

	CE_Normalize(finalMove);
	myCamera->Move(finalMove * cameraSpeed);*/

	const CE_Matrix44f& view = myCamera->GetNotInvertedView();
	if (myInput->KeyIsPressed(DIK_W))
		myCamera->Move(view.GetForward() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_S))
		myCamera->Move(-view.GetForward() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_A))
		myCamera->Move(-view.GetRight() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_D))
		myCamera->Move(view.GetRight() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_Q))
		myCamera->Move(-view.GetUp() * cameraSpeed);
	if (myInput->KeyIsPressed(DIK_E))
		myCamera->Move(view.GetUp() * cameraSpeed);

	if (myInput->KeyIsPressed(DIK_UP))
		myCamera->Rotate(CE_Matrix44f::CreateRotateAroundX(-rotationSpeed));
	if (myInput->KeyIsPressed(DIK_DOWN))
		myCamera->Rotate(CE_Matrix44f::CreateRotateAroundX(rotationSpeed));
	if (myInput->KeyIsPressed(DIK_LEFT))
		myCamera->Rotate(CE_Matrix44f::CreateRotateAroundY(-rotationSpeed));
	if (myInput->KeyIsPressed(DIK_RIGHT))
		myCamera->Rotate(CE_Matrix44f::CreateRotateAroundY(rotationSpeed));
}