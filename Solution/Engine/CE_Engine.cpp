#include "stdafx.h"

#include "CE_DirectX.h"
#include "CE_Engine.h"
#include "CE_WindowHandler.h"

CE_Engine::CE_Engine()
{
	myWindowHandler = new CE_WindowHandler(1280, 720);
	myDirectX = new CE_DirectX(myWindowHandler);
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
		myDirectX->FinishFrame();
	}
}
