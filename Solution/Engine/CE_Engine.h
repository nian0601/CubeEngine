#pragma once

class CE_WindowHandler;
class CE_GPUContext;

class CE_Shader;
class CE_Model;
class CE_Camera;

class CE_Engine
{
public:
	CE_Engine();
	~CE_Engine();

	void Run();

private:
	CE_WindowHandler* myWindowHandler;
	CE_GPUContext* myGPUContext;


	CE_Shader* myShader;
	CE_Model* myCube;
	CE_Camera* myCamera;
};

