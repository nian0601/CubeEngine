#include "stdafx.h"
#include "Game.h"

#include <CE_Camera.h>
#include <CE_Engine.h>
#include <CE_World.h>

#include <CUI_Manager.h>
#include <CUI_Image.h>
#include <CUI_VBox.h>
#include <CUI_HBox.h>
#include <CUI_Button.h>
#include <CUI_TreeView.h>
#include <CUI_Label.h>
#include <CUI_ValueController.h>

#include <CE_Font.h>
#include <CE_Input.h>
#include <CE_Window.h>
#include <CE_WindowManager.h>

#include "MovementComponent.h"
#include "TranslationComponent.h"

#include "AABBProcessor.h"
#include "BehaviorProcessor.h"
#include "CreateEntityProcessor.h"
#include "InputProcessor.h"
#include "MovementProcessor.h"
#include "RenderProcessor.h"
#include "SelectionProcessor.h"
#include "AIEventProcessor.h"

#include "EntityFactory.h"

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

	InputProcessor* inputProcessor = new InputProcessor(*myWorld, anEngine.GetInput());
	myWorld->AddProcessor(inputProcessor);

	CreateEntityProcessor* createProcessor = new CreateEntityProcessor(*myWorld, *myEntityFactory);
	myWorld->AddProcessor(createProcessor);

	SelectionProcessor* selectionProcessor = new SelectionProcessor(*myWorld, camera);
	myWorld->AddProcessor(selectionProcessor);

	myWorld->AddProcessor<MovementProcessor>();
	myWorld->AddProcessor<AABBProcessor>();
	myWorld->AddProcessor<BehaviorProcessor>();
	myWorld->AddProcessor<AIEventProcessor>();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	InitGUI();
	InitWorld();

	//myGUIWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 320, 320}, "GUI Window");
}

void Game::Update(float aDelta)
{
	myWorld->Update(aDelta);

	myUIManager->Update();
}

void Game::Render()
{
	//myUIManager->Render(myGUIWindow->GetRendererProxy());
}

void Game::InitWorld()
{
	CE_Entity player = myEntityFactory->InstansiateEntity(eEntityTypes::PLAYER);
	TranslationComponent& playerTranslate = myWorld->GetComponent<TranslationComponent>(player);
	playerTranslate.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));

	CE_Entity gatherer = myEntityFactory->InstansiateEntity(eEntityTypes::GATHERER);
	TranslationComponent& gathererTranslate = myWorld->GetComponent<TranslationComponent>(gatherer);
	gathererTranslate.myOrientation.SetPos(CE_Vector3f(7.f, 1.f, 1.f));

	CE_Entity water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate1 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate1.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate2 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate2.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 4.f));

	water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate3 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate3.myOrientation.SetPos(CE_Vector3f(4.f, 1.f, 4.f));


	CE_Entity stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate1 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate1.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 5.f));

	stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate2 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate2.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 4.f));

	stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate3 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate3.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 4.f));

	InitGrid();

	PopulateEntityTreeView(player);
}

void Game::InitGrid()
{
	const int gridSize = 10;
	for (int z = 0; z < gridSize; ++z)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			CE_Vector3f pos(static_cast<float>(x), 0.f, static_cast<float>(z));
	
			CE_Entity entity = myEntityFactory->InstansiateEntity(eEntityTypes::GROUND);
			TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(entity);
			translate.myOrientation.SetPos(pos);
		}
	}
}

void Game::InitGUI()
{
	CUI_TreeView* debugTree = new CUI_TreeView(*myFont, "+ Debug Menu");

	CUI_Button* reloadWorldButton = new CUI_Button(*myFont, "Reload World");
	reloadWorldButton->myOnClick = std::bind(&Game::OnClickFunction, this);
	debugTree->AddWidget(reloadWorldButton);

	myEntityTreeView = new CUI_TreeView(*myFont, "+ Entity Menu");
	myEntityTreeView->SetExpanded(false);

	myUIManager = new CUI_Manager(*myInput);
	myUIManager->AddWidget(debugTree);
	myUIManager->AddWidget(myEntityTreeView);
}

void Game::PopulateEntityTreeView(unsigned int anEntity)
{
	myEntityTreeView->DeleteAllChildren();

	CreatePositionWidget(anEntity);
	CreateMovementWidget(anEntity);
}

void Game::CreatePositionWidget(unsigned int anEntity)
{
	if (!myWorld->HasComponent<TranslationComponent>(anEntity))
		return;

	TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(anEntity);

	CUI_TreeView* positionView = new CUI_TreeView(*myFont, "Position");

	CUI_HBox* xPosBox = new CUI_HBox();
	xPosBox->AddWidget(new CUI_Label(*myFont, "X:  "));
	xPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[12])));

	CUI_HBox* yPosBox = new CUI_HBox();
	yPosBox->AddWidget(new CUI_Label(*myFont, "Y:  "));
	yPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[13])));

	CUI_HBox* zPosBox = new CUI_HBox();
	zPosBox->AddWidget(new CUI_Label(*myFont, "Z:  "));
	zPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[14])));

	positionView->AddWidget(xPosBox);
	positionView->AddWidget(yPosBox);
	positionView->AddWidget(zPosBox);

	myEntityTreeView->AddWidget(positionView);
}

void Game::CreateMovementWidget(unsigned int anEntity)
{
	if (!myWorld->HasComponent<MovementComponent>(anEntity))
		return;

	MovementComponent& movement = myWorld->GetComponent<MovementComponent>(anEntity);

	CUI_TreeView* movementView = new CUI_TreeView(*myFont, "Movement");

	CUI_HBox* speedBox = new CUI_HBox();
	speedBox->AddWidget(new CUI_Label(*myFont, "Speed:  "));
	speedBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&movement.mySpeed)));

	movementView->AddWidget(speedBox);

	myEntityTreeView->AddWidget(movementView);
}

void Game::OnClickFunction()
{
	myWorld->DestroyAllEntities();
	InitWorld();
}
