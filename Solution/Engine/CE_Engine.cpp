#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_Shader.h"
#include "CE_GPUContext.h"
#include "CE_Game.h"
#include "CE_Renderer.h"
#include "CE_Camera.h"
#include "CE_RendererProxy.h"

CE_Engine::CE_Engine(CE_Game* aGame)
	: myGame(aGame)
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myGPUContext = new CE_GPUContext(myWindowHandler);

	myRenderer = new CE_Renderer(*myGPUContext);
	myRendererProxy = new CE_RendererProxy(*myRenderer);

	myCamera = new CE_Camera(myWindowHandler->GetWindowSize());

	myGame->Init(*this);
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myRendererProxy);
	CE_SAFE_DELETE(myRenderer);
	CE_SAFE_DELETE(myGPUContext);
	CE_SAFE_DELETE(myWindowHandler);
}

void CE_Engine::Run()
{
	while (myWindowHandler->PumpEvent())
	{
		myGame->Update(1.f/60.f);
		myCamera->Update();

		myGame->Render(*myRendererProxy);

		myRenderer->Render(*myCamera);
		myGPUContext->EndFrame();
	}
}

const CE_Camera& CE_Engine::GetCamera() const
{
	return *myCamera;
}

CE_RendererProxy& CE_Engine::GetRendererProxy()
{
	return *myRendererProxy;
}

const CE_GPUContext& CE_Engine::GetGPUContext() const
{
	return *myGPUContext;
}

const CE_Vector2i& CE_Engine::GetWindowSize() const
{
	return myWindowHandler->GetWindowSize();
}
