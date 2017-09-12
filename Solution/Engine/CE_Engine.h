#pragma once

class CE_WindowHandler;
class CE_GPUContext;

class CE_Shader;
class CE_Cube;

#include "CE_Vector3.h"
#include "CE_Matrix44.h"

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
	CE_Cube* myCube;
	CE_Matrix44f myWorld;
	CE_Matrix44f myView;
	CE_Matrix44f myViewProjection;
	CE_Matrix44f myProjection;

	CE_Vector3i test;
};

