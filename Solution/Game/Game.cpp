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
#include "InputProcessor.h"
#include "CreateEntityProcessor.h"
#include "PlacingProcessor.h"
#include "MoverProcessor.h"

#include "RenderProcessor.h"
#include "RotationProcessor.h"
#include "MovementProcessor.h"
#include "CollisionProcessor.h"
#include "PickUpProcessor.h"
#include "EntityFactory.h"

#include <CUI_Manager.h>
#include <CUI_Image.h>
#include <CUI_VBox.h>
#include <CUI_HBox.h>
#include "..\Engine\CE_Input.h"

Game::Game()
{
}


Game::~Game()
{
	CE_SAFE_DELETE(myUIManager);
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
	myWorld->AddProcessor<MovementProcessor>();
	myWorld->AddProcessor<CollisionProcessor>();
	myWorld->AddProcessor<PickUpProcessor>();

	InputProcessor* inputProcessor = new InputProcessor(*myWorld, anEngine.GetInput());
	myWorld->AddProcessor(inputProcessor);

	CreateEntityProcessor* createProcessor = new CreateEntityProcessor(*myWorld, *myEntityFactory);
	myWorld->AddProcessor(createProcessor);
	myWorld->AddProcessor<PlacingProcessor>();
	myWorld->AddProcessor<MoverProcessor>();

	CreateGrid();

	CE_Entity pickup = myEntityFactory->InstansiateEntity(PICK_UP);
	TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(pickup);
	translate.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	CE_Entity player = myEntityFactory->InstansiateEntity(PLAYER);
	TranslationComponent& playerTranslate = myWorld->GetComponent<TranslationComponent>(player);
	playerTranslate.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));



	CUI_HBox* hbox = new CUI_HBox();
	hbox->AddWidget(new CUI_Image({ 0.25f, 0.25f }, { 1.f, 0.f, 0.f, 1.f }));
	hbox->AddWidget(new CUI_Image({ 0.1f, 0.25f }, { 0.f, 1.f, 0.f, 1.f }));
	myWidget = new CUI_Image({ 0.1f, 0.5f }, { 0.f, 0.f, 1.f, 1.f });
	hbox->AddWidget(myWidget);
	
	CUI_HBox* hbox2 = new CUI_HBox();
	hbox2->AddWidget(new CUI_Image({ 0.4f, 0.4f }, { 0.5f, 1.f, 1.f, 1.f }));
	hbox2->AddWidget(new CUI_Image({ 0.6f, 0.15f }, { 0.5f, 1.f, 0.5f, 1.f }));
	
	CUI_VBox* vbox = new CUI_VBox();
	vbox->AddWidget(hbox);
	vbox->AddWidget(hbox2);
	
	myUIManager = new CUI_Manager(anEngine.GetInput());
	myUIManager->AddWidget(vbox);

	//myWidget = new CUI_Image({ 0.25f, 0.25f }, { 1.f, 0.f, 0.f, 1.f });
	//myUIManager = new CUI_Manager(anEngine.GetInput());
	//myUIManager->AddWidget(myWidget);

	myInput = &anEngine.GetInput();
}

void Game::Update(float aDelta)
{
	myWorld->Update(aDelta);

	CE_Vector2f size = myWidget->GetSize();
	if (myInput->KeyDown(DIK_1))
		size.y = 0.1f;
	if (myInput->KeyDown(DIK_2))
		size.y = 0.3f;
	if (myInput->KeyDown(DIK_3))
		size.y = 0.5f;
	if (myInput->KeyDown(DIK_4))
		size.y = 0.7f;

	myWidget->SetSize(size);
	myUIManager->Update();
}

void Game::Render(CE_RendererProxy& anRendererProxy)
{
	myUIManager->Render(anRendererProxy);
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
