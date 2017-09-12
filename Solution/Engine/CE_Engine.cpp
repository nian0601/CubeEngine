#include "stdafx.h"

#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_Shader.h"
#include "CE_Model.h"
#include "CE_Cube.h"
#include "CE_GPUContext.h"

#define PI 3.14159265f

CE_Engine::CE_Engine()
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myGPUContext = new CE_GPUContext(myWindowHandler);

	myShader = new CE_Shader();
	myShader->Init(L"Data/Shaders/Cube.ce_shader", *myGPUContext);
	myCube = new CE_Cube();
	myCube->Init(*myGPUContext);

	myProjection = CE_Matrix44f::CreateProjectionMatrixLH(0.1f, 100.f, 720.f / 1280.f, PI * 0.5f);

	myView.SetPos(CE_Vector4f(0.f, 0.f, -5.f, 1.f));
	myView = CE_InverseSimple(myView);

	
	myViewProjection = myView * myProjection;
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
		myWorld = myWorld * CE_Matrix44f::CreateRotateAroundY(PI * 0.00001f);

		myCube->Render(*myGPUContext);
		myShader->Render(*myGPUContext, myCube->GetIndexCount(), myWorld, myViewProjection);
		myGPUContext->EndFrame();
	}
}
