#include "stdafx.h"
#include "Game.h"

#include <CE_Engine.h>
#include <CE_World.h>

#include "RenderComponent.h"
#include "TranslationComponent.h"
#include "RotationComponent.h"

#include "RenderProcessor.h"
#include "RotationProcessor.h"

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

	RenderProcessor* processor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(processor);
	myWorld->AddProcessor<RotationProcessor>();

	CE_Entity first = myWorld->CreateEntity();
	TranslationComponent& translate = myWorld->AddComponent<TranslationComponent>(first);
	translate.myOrientation.SetPos(CE_Vector3f(1.f, 2.f, 5.f));
	RenderComponent& render = myWorld->AddComponent<RenderComponent>(first);
	render.myColor = CE_Vector4f(1.f, 0.f, 0.f, 1.f);
	RotationComponent& rotation = myWorld->AddComponent<RotationComponent>(first);
	rotation.mySpeed = 3.14f;

	CE_Entity second = myWorld->CreateEntity();
	TranslationComponent& translate2 = myWorld->AddComponent<TranslationComponent>(second);
	translate2.myOrientation.SetPos(CE_Vector3f(0.f, 0.f, 5.f));
	RenderComponent& render2 = myWorld->AddComponent<RenderComponent>(second);
	render2.myColor = CE_Vector4f(0.f, 1.f, 0.f, 1.f);
}

void Game::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void Game::Render(CE_RendererProxy& /*anRendererProxy*/)
{

}
