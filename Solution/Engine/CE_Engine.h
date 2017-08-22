#pragma once

#include "CE_WindowHandler.h"

class CE_Engine
{
public:
	CE_Engine();
	~CE_Engine();

	void Run();

private:
	CE_WindowHandler myWindowHandler;
};

