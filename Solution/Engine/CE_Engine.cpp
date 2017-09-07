#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_Engine.h"
#include "CE_WindowHandler.h"
#include "CE_ColorShaderClass.h"
#include "CE_Model.h"

#define PI 3.14159265f

CE_Engine::CE_Engine()
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myDirectX = new CE_DirectX(myWindowHandler);

	myShader = new CE_ColorShaderClass();
	myShader->Init(myDirectX->myDevice);
	myModel = new CE_Model();
	myModel->Init(myDirectX->myDevice);

	myProjection = CE_Matrix44f::CreateProjectionMatrixLH(0.1f, 100.f, 720.f / 1280.f, PI * 0.5f);

	myView.SetPos(CE_Vector4f(0.f, 0.f, -2.f, 0.f));
	//myView = CE_InverseSimple(myView);
}


CE_Engine::~CE_Engine()
{
	CE_SAFE_DELETE(myDirectX);
	CE_SAFE_DELETE(myWindowHandler);
}

void CE_Engine::Run()
{
	while (myWindowHandler->PumpEvent())
	{
		myModel->Render(myDirectX->myDeviceContext);
		myShader->Render(myDirectX->myDeviceContext, myModel->GetIndexCount(), myWorld, myView, myProjection);
		myDirectX->FinishFrame();
	}
}
