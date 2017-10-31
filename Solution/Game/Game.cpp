#include "stdafx.h"
#include "Game.h"

#include <CE_Engine.h>

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

#include "InGameContext.h"

Game::Game()
{
}


Game::~Game()
{
	CE_SAFE_DELETE(myGameContext);
	CE_SAFE_DELETE(myUIManager);
}

void Game::Init(CE_Engine& anEngine)
{
	myInput = &anEngine.GetInput();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	InitGUI();

	myGameContext = new InGameContext();
	myGameContext->Init(anEngine);

	//myGUIWindow = CE_WindowManager::GetInstance()->CreateNewWindow({ 320, 320}, "GUI Window");
}

void Game::Update(float aDelta)
{
	//myUIManager->Update();

	myGameContext->Update(aDelta);
}

void Game::Render()
{
	//myUIManager->Render(myGUIWindow->GetRendererProxy());

	myGameContext->Render();
}

void Game::InitGUI()
{
	//CUI_TreeView* debugTree = new CUI_TreeView(*myFont, "+ Debug Menu");
	//
	//CUI_Button* reloadWorldButton = new CUI_Button(*myFont, "Reload World");
	//reloadWorldButton->myOnClick = std::bind(&Game::OnClickFunction, this);
	//debugTree->AddWidget(reloadWorldButton);
	//
	//myEntityTreeView = new CUI_TreeView(*myFont, "+ Entity Menu");
	//myEntityTreeView->SetExpanded(false);
	//
	//myUIManager = new CUI_Manager(*myInput);
	//myUIManager->AddWidget(debugTree);
	//myUIManager->AddWidget(myEntityTreeView);
}

void Game::PopulateEntityTreeView(unsigned int anEntity)
{
	anEntity;

	//myEntityTreeView->DeleteAllChildren();
	//
	//CreatePositionWidget(anEntity);
	//CreateMovementWidget(anEntity);
}

void Game::CreatePositionWidget(unsigned int anEntity)
{
	anEntity;
	//if (!myWorld->HasComponent<TranslationComponent>(anEntity))
	//	return;
	//
	//TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(anEntity);
	//
	//CUI_TreeView* positionView = new CUI_TreeView(*myFont, "Position");
	//
	//CUI_HBox* xPosBox = new CUI_HBox();
	//xPosBox->AddWidget(new CUI_Label(*myFont, "X:  "));
	//xPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[12])));
	//
	//CUI_HBox* yPosBox = new CUI_HBox();
	//yPosBox->AddWidget(new CUI_Label(*myFont, "Y:  "));
	//yPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[13])));
	//
	//CUI_HBox* zPosBox = new CUI_HBox();
	//zPosBox->AddWidget(new CUI_Label(*myFont, "Z:  "));
	//zPosBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&translation.myOrientation.myMatrix[14])));
	//
	//positionView->AddWidget(xPosBox);
	//positionView->AddWidget(yPosBox);
	//positionView->AddWidget(zPosBox);
	//
	//myEntityTreeView->AddWidget(positionView);
}

void Game::CreateMovementWidget(unsigned int anEntity)
{
	anEntity;

	//if (!myWorld->HasComponent<MovementComponent>(anEntity))
	//	return;
	//
	//MovementComponent& movement = myWorld->GetComponent<MovementComponent>(anEntity);
	//
	//CUI_TreeView* movementView = new CUI_TreeView(*myFont, "Movement");
	//
	//CUI_HBox* speedBox = new CUI_HBox();
	//speedBox->AddWidget(new CUI_Label(*myFont, "Speed:  "));
	//speedBox->AddWidget(new CUI_Label(*myFont, new CUI_ValueController(&movement.mySpeed)));
	//
	//movementView->AddWidget(speedBox);
	//
	//myEntityTreeView->AddWidget(movementView);
}

void Game::OnClickFunction()
{
	//myWorld->DestroyAllEntities();
	//InitWorld();
}
