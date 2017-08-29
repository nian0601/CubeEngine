#include "stdafx.h"
#include "CE_Engine.h"
#include "CE_WindowHandler.h"

CE_Engine::CE_Engine()
	: myWindowHandler(1280, 720)
{
	CE_ASSERT(false, "Test %f", 123.f);
}


CE_Engine::~CE_Engine()
{
}

void CE_Engine::Run()
{
	while (myWindowHandler.PumpEvent())
		;
}
