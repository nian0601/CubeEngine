#pragma once

class CE_DirectX;
class CE_WindowHandler;

class CE_ColorShaderClass;
class CE_Model;

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
	CE_DirectX* myDirectX;


	CE_ColorShaderClass* myShader;
	CE_Model* myModel;
	CE_Matrix44f myWorld;
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;

	CE_Vector3i test;
};

