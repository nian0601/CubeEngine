#include "stdafx.h"
#include "Game.h"

#include <CE_Engine.h>
#include <CE_World.h>

#include "RenderComponent.h"
#include "TranslationComponent.h"
#include "RotationComponent.h"

#include "RenderProcessor.h"
#include "RotationProcessor.h"
#include "..\Engine\CE_Camera.h"

Game::Game()
{
}


Game::~Game()
{
	CE_SAFE_DELETE(myWorld);
}

void Game::Init(CE_Engine& anEngine)
{
	myWorld = new CE_World();

	CE_Camera& camera = anEngine.GetCamera();
	camera.SetPosition(CE_Vector3f(5.f, 10.f, -5.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.25));

	RenderProcessor* processor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(processor);
	myWorld->AddProcessor<RotationProcessor>();

	CreateGrid();


	CE_Entity entity = myWorld->CreateEntity();
	TranslationComponent& translate = myWorld->AddComponent<TranslationComponent>(entity);
	RenderComponent& render = myWorld->AddComponent<RenderComponent>(entity);

	translate.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));
	render.myColor = CE_Vector4f(1.f, 0.f, 0.f, 1.f);
}

void Game::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void Game::Render(CE_RendererProxy& /*anRendererProxy*/)
{

}

void Game::CreateGrid()
{
	const int gridSize = 10;
	float color = 0.58f;
	for (int z = 0; z < gridSize; ++z)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			CE_Vector3f pos(static_cast<float>(x), 0.f, static_cast<float>(z));

			CE_Entity entity = myWorld->CreateEntity();
			TranslationComponent& translate = myWorld->AddComponent<TranslationComponent>(entity);
			RenderComponent& render = myWorld->AddComponent<RenderComponent>(entity);

			translate.myOrientation.SetPos(pos);
			render.myColor = CE_Vector4f(color, color, color, 1.f);
		}
	}
}
