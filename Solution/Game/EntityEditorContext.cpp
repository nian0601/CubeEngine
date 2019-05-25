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
#include <CUI_EditBox.h>

#include <CE_Window.h>

#include <CT_ToolModule.h>

#include <CE_DebugDraw.h>
#include <CE_FileSystem.h>
#include "EntityFactory.h"


EntityEditorContext::EntityEditorContext()
{
}


EntityEditorContext::~EntityEditorContext()
{
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

	myWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld);

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	myNumEntries = 0;

	InitGUI();
}

void EntityEditorContext::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void EntityEditorContext::Render()
{
	RenderGrid();
}

void EntityEditorContext::RenderGrid()
{
	const float cellSize = 1.f;
	const float halfCellSize = cellSize * 0.5f;
	const int numPerSide = 11;
	const CE_Vector4f color(0.3f, 0.3f, 0.3f, 1.f);

	int startX = -numPerSide / 2;
	int endX = numPerSide / 2;

	int startZ = -numPerSide / 2;
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

void EntityEditorContext::InitGUI()
{
	myTreeView = new CUI_TreeView("Components");
	myTreeView->SetExpanded(true);
	myUIManager->AddWidget(myTreeView);
	CE_Entity entity = myWorld->CreateEmptyEntity();
	myWorld->AddComponent<TranslationComponent>(entity);
	myRenderComponent = &myWorld->AddComponent<RenderComponent>(entity);
	myRenderComponent->myEntries.Respace(128);
	CreateRenderComponentWidget();
}


void EntityEditorContext::CreateRenderComponentWidget()
{
	myRenderComponentView = new CUI_TreeView("Render Component");
	myRenderComponentView->SetExpanded(true);
	myTreeView->AddWidget(myRenderComponentView);
	

	CUI_Button* addEntryButton = new CUI_Button("Add Entry");
	addEntryButton->myOnClick = std::bind(&EntityEditorContext::AddRenderEntry, this);
	myRenderComponentView->AddWidget(addEntryButton);

	CUI_Button* clearEntriesButton = new CUI_Button("Clear Entries");
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
	CUI_TreeView* entryView = new CUI_TreeView(text);

	entryView->AddWidget(CreateVectorWidget("Scale", entry.myScale));
	entryView->AddWidget(CreateColorWidget("Color", entry.myColor));

	myRenderComponentView->AddWidget(entryView);
}

CUI_TreeView* EntityEditorContext::CreateVectorWidget(const char* aText, CE_Vector3f& aVector)
{
	CUI_HBox* xBox = CreateFloatController("X: ", aVector.x);
	CUI_HBox* yBox = CreateFloatController("Y: ", aVector.y);
	CUI_HBox* zBox = CreateFloatController("Z: ", aVector.z);

	CUI_TreeView* view = new CUI_TreeView(aText);
	view->AddWidget(xBox);
	view->AddWidget(yBox);
	view->AddWidget(zBox);
	return view;
}

CUI_TreeView* EntityEditorContext::CreateColorWidget(const char* aText, CE_Vector4f& aVector)
{
	CUI_HBox* rBox = CreateFloatController("R: ", aVector.x);
	CUI_HBox* gBox = CreateFloatController("G: ", aVector.y);
	CUI_HBox* bBox = CreateFloatController("B: ", aVector.z);

	CUI_TreeView* view = new CUI_TreeView(aText);
	view->AddWidget(rBox);
	view->AddWidget(gBox);
	view->AddWidget(bBox);
	return view;
}

CUI_HBox* EntityEditorContext::CreateFloatController(const char* aText, float& aValue)
{
	CUI_HBox* box = new CUI_HBox();
	box->AddWidget(new CUI_Label(aText));

	CUI_ValueController* controller = new CUI_ValueController(&aValue);
	box->AddWidget(new CUI_Label(controller));

	return box;
}

void EntityEditorContext::ClearRenderEntries()
{
	myRenderComponentView->DeleteChildren(2);
	myRenderComponent->myEntries.RemoveAll();
}