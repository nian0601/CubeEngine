#include "stdafx.h"
#include "EntityEditorContext.h"

#include <CE_Engine.h>
#include <CE_Font.h>

#include <CUI_Manager.h>

EntityEditorContext::EntityEditorContext()
{
}


EntityEditorContext::~EntityEditorContext()
{
	CE_SAFE_DELETE(myUIManager);
}

void EntityEditorContext::Init(CE_Engine& anEngine)
{
	myInput = &anEngine.GetInput();
	myRendererProxy = &anEngine.GetRendererProxy();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	InitGUI();
}

void EntityEditorContext::Update(float aDelta)
{
	aDelta;

	myUIManager->Update();
}

void EntityEditorContext::Render()
{
	myUIManager->Render(*myRendererProxy);
}

void EntityEditorContext::InitGUI()
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

void EntityEditorContext::PopulateEntityTreeView(unsigned int anEntity)
{
	anEntity;
	//myEntityTreeView->DeleteAllChildren();
	//
	//CreatePositionWidget(anEntity);
	//CreateMovementWidget(anEntity);
}

void EntityEditorContext::CreatePositionWidget(unsigned int anEntity)
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

void EntityEditorContext::CreateMovementWidget(unsigned int anEntity)
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

void EntityEditorContext::OnClickFunction()
{
	//myWorld->DestroyAllEntities();
	//InitWorld();
}
