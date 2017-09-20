#include "stdafx.h"
#include "Game.h"

#include <CE_Camera.h>
#include <CE_Engine.h>
#include <CE_World.h>

#include "RenderComponent.h"
#include "TranslationComponent.h"
#include "RotationComponent.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "PickUpComponent.h"
#include "InventoryComponent.h"

#include "RenderProcessor.h"
#include "RotationProcessor.h"
#include "InputProcessor.h"
#include "CollisionProcessor.h"
#include "PickUpProcessor.h"
#include "EntityFactory.h"

Game::Game()
{
}


Game::~Game()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myTemplateWorld);
	CE_SAFE_DELETE(myWorld);
}

void Game::Init(CE_Engine& anEngine)
{
	myWorld = new CE_World();
	myTemplateWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld, *myTemplateWorld);

	CE_Camera& camera = anEngine.GetCamera();
	camera.SetPosition(CE_Vector3f(5.f, 10.f, -5.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.25));

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);
	myWorld->AddProcessor<RotationProcessor>();

	InputProcessor* inputProcessor = new InputProcessor(*myWorld, anEngine.GetInput());
	myWorld->AddProcessor(inputProcessor);
	myWorld->AddProcessor<CollisionProcessor>();
	myWorld->AddProcessor<PickUpProcessor>();

	CreateGrid();

	CE_Entity pickup = myEntityFactory->InstansiateEntity(PICK_UP);
	TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(pickup);
	translate.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	CE_Entity player = myEntityFactory->InstansiateEntity(PLAYER);
	TranslationComponent& playerTranslate = myWorld->GetComponent<TranslationComponent>(player);
	playerTranslate.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));
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
	for (int z = 0; z < gridSize; ++z)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			CE_Vector3f pos(static_cast<float>(x), 0.f, static_cast<float>(z));

			CE_Entity entity = myEntityFactory->InstansiateEntity(GROUND);
			TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(entity);
			translate.myOrientation.SetPos(pos);
		}
	}
}
