#include "stdafx.h"
#include "Game.h"

#include <CE_Shader.h>
#include <CE_Model.h>
#include <CE_Camera.h>
#include <CE_Engine.h>

Game::Game()
{
}


Game::~Game()
{
}

void Game::Init(CE_Engine& anEngine)
{
	myCamera = new CE_Camera(anEngine.GetWindowSize());

	myShader = new CE_Shader();
	myShader->Init(L"Data/Shaders/Cube.ce_shader", anEngine.GetGPUContext());

	myCube1 = new CE_Model();
	myCube1->InitCube(anEngine.GetGPUContext());
	myCube1->SetPosition(CE_Vector3f(1.f, 2.f, 5.f));
	myCube1->SetColor(CE_Vector4f(1.f, 0.f, 0.f, 1.f));

	myCube2 = new CE_Model();
	myCube2->InitCube(anEngine.GetGPUContext());
	myCube2->SetPosition(CE_Vector3f(0.f, 0.f, 5.f));
	myCube2->SetColor(CE_Vector4f(0.f, 1.f, 0.f, 1.f));
}

#define PI 3.14159265f
void Game::Update()
{
	myCube1->Rotate(CE_Matrix44f::CreateRotateAroundY(PI * 0.00001f));
}

void Game::Render(const CE_GPUContext& anGPUContext)
{
	myShader->SetGlobalGPUData(anGPUContext, *myCamera);
	myCube1->Render(anGPUContext);
	myCube2->Render(anGPUContext);
}
