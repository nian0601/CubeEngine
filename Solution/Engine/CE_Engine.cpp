#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_CubeShader.h"
#include "CE_GPUContext.h"
#include "CE_Game.h"
#include "CE_Renderer.h"
#include "CE_Camera.h"
#include "CE_RendererProxy.h"
#include "CE_Time.h"
#include "CE_Input.h"

CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CE_ASSERT(FAILED(hr) == false, "Failed to CoInitializeEx");

	myWindowHandler = new CE_WindowHandler(1280, 720);
	myGPUContext = new CE_GPUContext(myWindowHandler);

	myRenderer = new CE_Renderer(*myGPUContext);
	myRendererProxy = new CE_RendererProxy(*myRenderer);

	myCamera = new CE_Camera(myWindowHandler->GetWindowSize());

	myTime = new CE_Time();
	myInput = new CE_Input(myWindowHandler->GetHWND(), GetModuleHandle(NULL), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

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
	CE_SAFE_DELETE(myWindowHandler);
}

void CE_Engine::Run()
{
	while (myWindowHandler->PumpEvent())
	{
		myTime->Update();
		myInput->Update();

		if (myInput->KeyDown(DIK_ESCAPE))
			return;

		myGame->Update(myTime->GetFrameTime());
		myCamera->Update();

		myGame->Render(*myRendererProxy);

		myRenderer->Render(*myCamera);
		myGPUContext->EndFrame();
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
	return myWindowHandler->GetWindowSize();
}
