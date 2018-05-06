#include "stdafx.h"

#include <CE_Blackboard.h>
#include <CE_Camera.h>
#include <CE_DebugDraw.h>
#include <CE_Engine.h>
#include <CE_FileSystem.h>
#include <CE_Font.h>
#include <CE_PathFinder.h>
#include <CE_World.h>
#include <CE_Window.h>

#include <CT_ToolModule.h>

#include <CUI_Button.h>
#include <CUI_Manager.h>
#include <CUI_EditBox.h>
#include <CUI_Dropbox.h>
#include <CUI_HBox.h>

#include "EntityFactory.h"
#include "LevelEditorContext.h"
#include "RenderProcessor.h"
#include "TranslationComponent.h"

LevelEditorContext::LevelEditorContext()
{
	CE_TYPE_REGISTER(float);
	CE_TYPE_REGISTER(CE_Vector3f);
	CE_TYPE_REGISTER(CE_Vector4f);
	CE_TYPE_REGISTER(CE_World*);
	CE_TYPE_REGISTER(double);
	CE_TYPE_REGISTER(CE_PathFinder*);
	CE_TYPE_REGISTER(CE_Blackboard*);
}


LevelEditorContext::~LevelEditorContext()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
	CE_SAFE_DELETE(myToolModule);
	CE_SAFE_DELETE(myFont);
}

void LevelEditorContext::Init(CE_Engine& anEngine)
{
	myRendererProxy = &anEngine.GetRendererProxy();

	myFont = new CE_Font();
	myFont->LoadFromFile("Data/Font/Decent_Font.png", anEngine.GetGPUContext());

	CE_Window& mainWindow = anEngine.GetMainWindow();
	InitGUI(mainWindow.GetUIManager());

	CE_Camera& camera = *mainWindow.GetCamera();
	camera.SetPosition(CE_Vector3f(0.f, 10.f, -7.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.30f));

	myToolModule = new CT_ToolModule(camera, anEngine.GetInput());

	InitWorld(anEngine);
	myEntityFactory = new EntityFactory(*myWorld);
}

void LevelEditorContext::Update(float aDelta)
{
	myToolModule->Update(aDelta);
	myWorld->Update(aDelta);
}

void LevelEditorContext::Render()
{
	RenderGrid();
	myToolModule->Render(*myRendererProxy);
}

void LevelEditorContext::RenderGrid()
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

void LevelEditorContext::InitWorld(CE_Engine& anEngine)
{
	myWorld = new CE_World();
	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);
}

void LevelEditorContext::InitGUI(CUI_Manager& aUIManager)
{
	BuildEntityDropbox(aUIManager);
}

void LevelEditorContext::BuildEntityDropbox(CUI_Manager& aUIManager)
{
	CE_GrowingArray<CE_FileSystem::FileInfo> entityFiles;
	CE_FileSystem::GetAllFilesFromDirectory("Data/Entities", entityFiles);

	CUI_Dropbox* dropbox = new CUI_Dropbox(*myFont, "Entities");
	for (const CE_FileSystem::FileInfo& file : entityFiles)
		dropbox->AddLabel(file.myFileNameNoExtention.c_str());

	dropbox->myOnSelection = std::bind(&LevelEditorContext::OnSelection, this, std::placeholders::_1);

	aUIManager.AddWidget(dropbox);

	CUI_HBox* saveBox = new CUI_HBox();
	myEditbox = new CUI_EditBox(*myFont, 256.f);
	CUI_Button* saveButton = new CUI_Button(*myFont, "Save Level");
	saveButton->myOnClick = std::bind(&LevelEditorContext::OnSaveLevel, this);

	saveBox->AddWidget(myEditbox);
	saveBox->AddWidget(saveButton);
	aUIManager.AddWidget(saveBox);
}

void LevelEditorContext::OnSelection(CUI_Widget* aWidget)
{
	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	const CE_String& text = label->GetText();

	CE_Entity entity = myEntityFactory->InstansiateEntity(text.c_str());
	if (myWorld->HasComponent<TranslationComponent>(entity) != -1)
	{
		TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(entity);
		myToolModule->AddToolEntity(entity, &translation.myOrientation, &translation.myScale);
	}

	EntityInfo& info = myEntityInfos.Add();
	info.myType = text;
	info.myID = entity;
}

void LevelEditorContext::OnSaveLevel()
{

}
