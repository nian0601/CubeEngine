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
#include <CUI_Dropbox.h>

#include <CE_Window.h>

#include <CT_Gizmo.h>
#include <CT_ToolModule.h>

#include <CE_DebugDraw.h>
#include <CE_FileSystem.h>
#include "EntityFactory.h"

#include <CE_PathFinder.h>
#include <CE_Blackboard.h>

EntityEditorContext::EntityEditorContext()
{
	CE_TYPE_REGISTER(float);
	CE_TYPE_REGISTER(CE_Vector3f);
	CE_TYPE_REGISTER(CE_Vector4f);
	CE_TYPE_REGISTER(CE_World*);
	CE_TYPE_REGISTER(double);
	CE_TYPE_REGISTER(CE_PathFinder*);
	CE_TYPE_REGISTER(CE_Blackboard*);
}


EntityEditorContext::~EntityEditorContext()
{
	CE_SAFE_DELETE(myToolModule);
}

void EntityEditorContext::Init(CE_Engine& anEngine)
{
	CE_Window& mainWindow = anEngine.GetMainWindow();
	CE_Camera* camera = mainWindow.GetCamera();
	camera->SetPosition(CE_Vector3f(0.f, 10.f, -5.f));
	camera->Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.30f));

	myUIManager = &mainWindow.GetUIManager();
	myInput = &anEngine.GetInput();
	myRendererProxy = &anEngine.GetRendererProxy();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	myWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld);

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	myNumEntries = 0;

	myToolModule = new CT_ToolModule(*camera, *myInput);
	InitGUI();
	SetupTestEntities();
}

void EntityEditorContext::Update(float aDelta)
{
	myToolModule->Update(aDelta);
	myWorld->Update(aDelta);
}

void EntityEditorContext::Render()
{
	RenderGrid();

	myToolModule->Render(*myRendererProxy);
}

void EntityEditorContext::RenderGrid()
{
	const float cellSize = 1.f;
	const float halfCellSize = cellSize * 0.5f;
	const int numPerSide = 11;
	const CE_Vector4f color(0.3f, 0.3f, 0.3f, 1.f);
	
	int startX = -numPerSide / 2;
	int endX = numPerSide / 2;

	int startZ= -numPerSide / 2;
	int endZ = numPerSide / 2;

	for (int z = startZ; z < endZ; ++z)
	{
		for (int x = startX; x < endX; ++x)
		{
			CE_Vector3f horizontal1(x * cellSize + halfCellSize, 0.f, z * cellSize + halfCellSize);
	
			if (x < endX - 1)
			{
				CE_Vector3f horizontal2((x + 1) * cellSize + halfCellSize, 0.f, z * cellSize + halfCellSize);
				CE_DRAW_LINE_COLOR(horizontal1, horizontal2, color);
			}
	
			if (z < endZ - 1)
			{
				CE_Vector3f verticalPos(x * cellSize + halfCellSize, 0.f, (z + 1) * cellSize + halfCellSize);
				CE_DRAW_LINE_COLOR(horizontal1, verticalPos, color);
			}
		}
	}
}

void EntityEditorContext::SetupTestEntities()
{
	CE_Entity entity = myWorld->CreateEmptyEntity();
	RenderComponent& renderComponent = myWorld->AddComponent<RenderComponent>(entity);
	RenderComponent::Entry& entry = renderComponent.myEntries.Add();
	entry.myScale = CE_Vector3f(1.f);
	entry.myColor = CE_Vector4f(0.25f, 0.5f, 0.75f, 1.f);

	TranslationComponent& translation = myWorld->AddComponent<TranslationComponent>(entity);
	myToolModule->AddToolEntity(entity, &translation.myOrientation, &translation.myScale);

	CE_Entity entity2 = myWorld->CreateEmptyEntity();
	RenderComponent& renderComponent2 = myWorld->AddComponent<RenderComponent>(entity2);
	RenderComponent::Entry& entry2 = renderComponent2.myEntries.Add();
	entry2.myScale = CE_Vector3f(1.f);
	entry2.myColor = CE_Vector4f(0.75f, 0.5f, 0.75f, 1.f);

	TranslationComponent& translation2 = myWorld->AddComponent<TranslationComponent>(entity2);
	translation2.myOrientation.SetPos(2.f, 0.f, 0.f);
	myToolModule->AddToolEntity(entity2, &translation2.myOrientation, &translation2.myScale);
}

void EntityEditorContext::InitGUI()
{
	//myTreeView = new CUI_TreeView(*myFont, "Components");
	//myTreeView->SetExpanded(true);
	//myUIManager->AddWidget(myTreeView);
	//CE_Entity entity = myWorld->CreateEmptyEntity();
	//myWorld->AddComponent<TranslationComponent>(entity);
	//myRenderComponent = &myWorld->AddComponent<RenderComponent>(entity);
	//myRenderComponent->myEntries.Respace(128);
	//CreateRenderComponentWidget()

	BuildEntityDropbox();
}

void EntityEditorContext::BuildEntityDropbox()
{
	CE_GrowingArray<CE_FileSystem::FileInfo> entityFiles;
	CE_FileSystem::GetAllFilesFromDirectory("Data/Entities", entityFiles);

	CUI_Dropbox* dropbox = new CUI_Dropbox(*myFont, "Entities");
	for (const CE_FileSystem::FileInfo& file : entityFiles)
		dropbox->AddLabel(file.myFileNameNoExtention.c_str());

	dropbox->myOnSelection = std::bind(&EntityEditorContext::OnSelection, this, std::placeholders::_1);

	myUIManager->AddWidget(dropbox);
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

void EntityEditorContext::OnSelection(CUI_Widget* aWidget)
{
	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	const CE_String& text = label->GetText();
	
	CE_Entity entity = myEntityFactory->InstansiateEntity(text.c_str());
	if (myWorld->HasComponent<TranslationComponent>(entity))
	{
		TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(entity);
		myToolModule->AddToolEntity(entity, &translation.myOrientation, &translation.myScale);
	}
}
