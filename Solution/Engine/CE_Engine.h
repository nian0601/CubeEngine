#pragma once

class CE_DirectX;
class CE_WindowHandler;

class CE_Engine
{
public:
	CE_Engine();
	~CE_Engine();

	void Run();

private:
	CE_WindowHandler* myWindowHandler;
	CE_DirectX* myDirectX;
};

