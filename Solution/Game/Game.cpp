#include "stdafx.h"
#include "Game.h"

#include <CE_Shader.h>
#include <CE_Model.h>
#include <CE_Camera.h>
#include <CE_Engine.h>
#include <CE_RendererProxy.h>

Game::Game()
{
}


Game::~Game()
{
}

void Game::Init(CE_Engine& /*anEngine*/)
{
	myWorldMatrix.SetPos(CE_Vector3f(1.f, 2.f, 5.f));
}

#define PI 3.14159265f
void Game::Update()
{
	CE_Vector3f pos = myWorldMatrix.GetPos();
	myWorldMatrix.SetPos(CE_Vector3f());
	myWorldMatrix = myWorldMatrix * CE_Matrix44f::CreateRotateAroundY(PI * 0.00001f);
	myWorldMatrix.SetPos(pos);
}

void Game::Render(CE_RendererProxy& anRendererProxy)
{
	anRendererProxy.AddCubeData(myWorldMatrix, CE_Vector4f(1.f, 0.f, 0.f, 1.f));

	CE_Matrix44f orientation;
	orientation.SetPos(CE_Vector3f(0.f, 0.f, 5.f));
	anRendererProxy.AddCubeData(orientation, CE_Vector4f(0.f, 1.f, 0.f, 1.f));
}
