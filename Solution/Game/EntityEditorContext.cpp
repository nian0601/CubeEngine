#include "stdafx.h"
#include "EntityEditorContext.h"

#include <CE_Engine.h>
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
#include <CE_ObjManager.h>
#include <CE_Input.h>


EntityEditorContext::EntityEditorContext()
{
}


EntityEditorContext::~EntityEditorContext()
{
	CE_SAFE_DELETE(myToolModule);
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
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

	myToolModule = new CT_ToolModule(*camera, *myInput);

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	myEntity = myWorld->CreateEmptyEntity();
	myWorld->AddComponent<TranslationComponent>(myEntity);
	myRenderComponent = &myWorld->AddComponent<RenderComponent>(myEntity);
	myRenderComponent->myEntries.Respace(128);

	SetupConstantWidgets();
}

void EntityEditorContext::Update(float aDelta)
{
	myToolModule->Update(aDelta);
	myWorld->Update(aDelta);

	if (myInput->MouseDown(1))
	{
		myModelsDropbox->SetPosition(myInput->GetMousePosition());
		myModelsDropbox->Show();
		myModelsDropbox->SetExpansion(true);
	}
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

void EntityEditorContext::SetupConstantWidgets()
{
	myModelsDropbox= new CUI_Dropbox("Models");
	myModelsDropbox->myOnSelection = std::bind(&EntityEditorContext::OnModelDropboxSelection, this, std::placeholders::_1, std::placeholders::_2);
	myUIManager->AddWidget(myModelsDropbox);

	CE_GrowingArray<CE_FileSystem::FileInfo> models;
	CE_FileSystem::GetAllFilesFromDirectory("Data/Models/", models);
	
	for (const CE_FileSystem::FileInfo& fileInfo : models)
		myModelsDropbox->AddLabel(fileInfo.myFileNameNoExtention.c_str());
}

void EntityEditorContext::OnModelDropboxSelection(CUI_Widget* aWidget, int /*aWidgetIndex*/)
{
	myModelsDropbox->SetExpansion(false);

	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	CE_String text = label->GetText();
	text += ".obj";

	RenderComponent::Entry& entry = myRenderComponent->myEntries.Add();
	entry.myModelID = CE_ObjManager::GetInstance()->GetObjID(text.c_str());;
	entry.myType = ModelType::eType::OBJ_MODEL;

	const CE_ObjData* objData = CE_ObjManager::GetInstance()->GetObjData(entry.myModelID);
	myToolModule->AddToolEntity(myEntity, &entry.myOffsetMatrix, &entry.myScale, objData->myMin, objData->myMax);
}
