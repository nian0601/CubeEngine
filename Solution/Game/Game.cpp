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
#include <CUI_Button.h>
#include <CUI_TreeView.h>
#include <CUI_Label.h>

#include <CE_Input.h>
#include <CE_Font.h>

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
	myInput = &anEngine.GetInput();

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

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	InitWorld();
	InitGUI();
}

void Game::Update(float aDelta)
{
	myWorld->Update(aDelta);

	myUIManager->Update();
}

void Game::Render(CE_RendererProxy& anRendererProxy)
{
	myUIManager->Render(anRendererProxy);
}

void Game::InitWorld()
{
	InitGrid();

	CE_Entity pickup = myEntityFactory->InstansiateEntity(PICK_UP);
	TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(pickup);
	translate.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	CE_Entity player = myEntityFactory->InstansiateEntity(PLAYER);
	TranslationComponent& playerTranslate = myWorld->GetComponent<TranslationComponent>(player);
	playerTranslate.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));
}

void Game::InitGrid()
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

void Game::InitGUI()
{
	//CUI_Button* button = new CUI_Button({ 0.10f, 0.10f }, { 0.7f, 0.7f, 0.7f, 1.f });
	//button->myOnClick = std::bind(&Game::OnClickFunction, this);
	//
	//CUI_VBox* vbox = new CUI_VBox();
	//vbox->AddWidget(button);
	//
	//myUIManager = new CUI_Manager(*myInput);
	//myUIManager->AddWidget(vbox);


	CUI_TreeView* treeview = new CUI_TreeView(*myFont, "+ Debug Menu");

	//CUI_Button* button = new CUI_Button({ 150.f, 50.f }, { 0.7f, 0.7f, 0.7f, 1.f });
	CUI_Button* button = new CUI_Button(*myFont, "Reload World");
	button->myOnClick = std::bind(&Game::OnClickFunction, this);
	treeview->AddWidget(button);

	CUI_Label* lable = new CUI_Label(*myFont, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	treeview->AddWidget(lable);

	CUI_TreeView* childTree = new CUI_TreeView();
	CUI_Image* img = new CUI_Image({ 250.f, 85.f }, { 0.1f, 1.f, 0.1f, 1.f });
	childTree->AddWidget(img);

	treeview->AddWidget(childTree);

	myUIManager = new CUI_Manager(*myInput);
	myUIManager->AddWidget(treeview);
}

void Game::OnClickFunction()
{
	myWorld->DestroyAllEntities();
	InitWorld();
}
