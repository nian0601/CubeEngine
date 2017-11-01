#include "stdafx.h"
#include "EntityEditorContext.h"

#include <CE_Engine.h>
#include <CE_Font.h>
#include <CE_World.h>
#include <CE_Camera.h>

#include "RenderProcessor.h"
#include "RenderComponent.h"
#include "TranslationComponent.h"

#include <CUI_Manager.h>
#include <CUI_TreeView.h>
#include <CUI_HBox.h>
#include <CUI_Label.h>
#include <CUI_Button.h>
#include <CUI_ValueController.h>

EntityEditorContext::EntityEditorContext()
{
}


EntityEditorContext::~EntityEditorContext()
{
	CE_SAFE_DELETE(myUIManager);
}

void EntityEditorContext::Init(CE_Engine& anEngine)
{
	CE_Camera& camera = anEngine.GetCamera();
	camera.SetPosition(CE_Vector3f(0.f, 10.f, -5.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.30f));

	myInput = &anEngine.GetInput();
	myRendererProxy = &anEngine.GetRendererProxy();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	myWorld = new CE_World();

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	myNumEntries = 0;
	InitGUI();
}

void EntityEditorContext::Update(float aDelta)
{
	myWorld->Update(aDelta);
	myUIManager->Update();
}

void EntityEditorContext::Render()
{
	myUIManager->Render(*myRendererProxy);
}

void EntityEditorContext::InitGUI()
{
	myTreeView = new CUI_TreeView(*myFont, "Components");
	myTreeView->SetExpanded(true);

	myUIManager = new CUI_Manager(*myInput);
	myUIManager->AddWidget(myTreeView);

	CE_Entity entity = myWorld->CreateEmptyEntity();
	myWorld->AddComponent<TranslationComponent>(entity);

	myRenderComponent = &myWorld->AddComponent<RenderComponent>(entity);
	myRenderComponent->myEntries.Respace(128);
	CreateRenderComponentWidget();
}

void EntityEditorContext::CreateRenderComponentWidget()
{
	myRenderComponentView = new CUI_TreeView(*myFont, "Render Component");
	myRenderComponentView->SetExpanded(true);
	myTreeView->AddWidget(myRenderComponentView);
	

	CUI_Button* addEntryButton = new CUI_Button(*myFont, "Add Entry");
	addEntryButton->myOnClick = std::bind(&EntityEditorContext::AddRenderEntry, this);
	myRenderComponentView->AddWidget(addEntryButton);

	CUI_Button* clearEntriesButton = new CUI_Button(*myFont, "Clear Entries");
	clearEntriesButton->myOnClick = std::bind(&EntityEditorContext::ClearRenderEntries, this);
	myRenderComponentView->AddWidget(clearEntriesButton);


	AddRenderEntry();
	AddRenderEntry();
}

void EntityEditorContext::AddRenderEntry()
{
	RenderComponent::Entry& entry = myRenderComponent->myEntries.Add();
	entry.myScale = CE_Vector3f(1.f);
	entry.myColor = CE_Vector4f(0.25f, 0.5f, 0.75f, 1.f);

	CE_String text = "Entry ";
	text += myNumEntries++;
	CUI_TreeView* entryView = new CUI_TreeView(*myFont, text);

	entryView->AddWidget(CreateVectorWidget("Scale", entry.myScale));
	entryView->AddWidget(CreateColorWidget("Color", entry.myColor));

	myRenderComponentView->AddWidget(entryView);
}

CUI_TreeView* EntityEditorContext::CreateVectorWidget(const char* aText, CE_Vector3f& aVector)
{
	CUI_HBox* xBox = CreateFloatController("X:   ", aVector.x);
	CUI_HBox* yBox = CreateFloatController("Y:   ", aVector.y);
	CUI_HBox* zBox = CreateFloatController("Z:   ", aVector.z);

	CUI_TreeView* view = new CUI_TreeView(*myFont, aText);
	view->AddWidget(xBox);
	view->AddWidget(yBox);
	view->AddWidget(zBox);
	return view;
}

CUI_TreeView* EntityEditorContext::CreateColorWidget(const char* aText, CE_Vector4f& aVector)
{
	CUI_HBox* rBox = CreateFloatController("R:   ", aVector.x);
	CUI_HBox* gBox = CreateFloatController("G:   ", aVector.y);
	CUI_HBox* bBox = CreateFloatController("B:   ", aVector.z);

	CUI_TreeView* view = new CUI_TreeView(*myFont, aText);
	view->AddWidget(rBox);
	view->AddWidget(gBox);
	view->AddWidget(bBox);
	return view;
}

CUI_HBox* EntityEditorContext::CreateFloatController(const char* aText, float& aValue)
{
	CUI_HBox* box = new CUI_HBox();
	box->AddWidget(new CUI_Label(*myFont, aText));

	CUI_ValueController* controller = new CUI_ValueController(&aValue);
	box->AddWidget(new CUI_Label(*myFont, controller));
	AddModifyButtons(controller, box);

	return box;
}

void EntityEditorContext::AddModifyButtons(CUI_ValueController* aController, CUI_HBox* aParent)
{
	CUI_Button* incrementButton = new CUI_Button(*myFont, " + ");
	incrementButton->myOnClick = std::bind(&EntityEditorContext::ModifyValueController, this, aController, 0.1f);
	aParent->AddWidget(incrementButton);

	CUI_Button* decrementButton = new CUI_Button(*myFont, " - ");
	decrementButton->myOnClick = std::bind(&EntityEditorContext::ModifyValueController, this, aController, -0.1f);
	aParent->AddWidget(decrementButton);
}

void EntityEditorContext::ModifyValueController(CUI_ValueController* aController, float aModifier)
{
	float value = aController->Get();

	value += aModifier;
	value = CE_Min(value, 1.f);
	value = CE_Max(value, 0.f);

	aController->Set(value);
}

void EntityEditorContext::ClearRenderEntries()
{
	myRenderComponentView->DeleteChildren(2);
	myRenderComponent->myEntries.RemoveAll();
}
