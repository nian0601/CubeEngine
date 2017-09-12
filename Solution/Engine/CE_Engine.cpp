#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_Shader.h"
#include "CE_Model.h"
#include "CE_GPUContext.h"

#define PI 3.14159265f

CE_Engine::CE_Engine()
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myGPUContext = new CE_GPUContext(myWindowHandler);

	myShader = new CE_Shader();
	myShader->Init(L"Data/Shaders/Cube.ce_shader", *myGPUContext);
	myCube = new CE_Model();
	myCube->InitCube(*myGPUContext);

	myProjection = CE_Matrix44f::CreateProjectionMatrixLH(0.1f, 100.f, 720.f / 1280.f, PI * 0.5f);

	//myView.SetPos(CE_Vector3f(0.f, -5.f, -15.f));
	myView = CE_InverseSimple(myView);

	myWorld.SetPos(CE_Vector3f(1.f, 2.f, 5.f));
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
		CE_Vector3f pos = myWorld.GetPos();
		myWorld.SetPos(CE_Vector3f());
		myWorld = CE_Matrix44f::CreateRotateAroundY(PI * 0.00001f) * myWorld;
		myWorld.SetPos(pos);

		myCube->Render(*myGPUContext); 
		myShader->Render(*myGPUContext, myCube->GetIndexCount(), myWorld, myView, myProjection);
		myGPUContext->EndFrame();
	}
}
