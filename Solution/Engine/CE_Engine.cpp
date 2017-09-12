#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_Shader.h"
#include "CE_Model.h"
#include "CE_GPUContext.h"
#include "CE_Camera.h"

#define PI 3.14159265f

CE_Engine::CE_Engine()
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myGPUContext = new CE_GPUContext(myWindowHandler);

	myShader = new CE_Shader();
	myShader->Init(L"Data/Shaders/Cube.ce_shader", *myGPUContext);
	myCube = new CE_Model();
	myCube->InitCube(*myGPUContext);
	myCamera = new CE_Camera(myWindowHandler->GetWindowSize());

	myCube->SetPosition(CE_Vector3f(1.f, 2.f, 5.f));
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myGPUContext);
	CE_SAFE_DELETE(myWindowHandler);
}

void CE_Engine::Run()
{
	while (myWindowHandler->PumpEvent())
	{
		myCube->Rotate(CE_Matrix44f::CreateRotateAroundY(PI * 0.00001f));

		myCube->Render(*myGPUContext); 
		myShader->Render(*myGPUContext, *myCube, *myCamera);
		myGPUContext->EndFrame();
	}
}
