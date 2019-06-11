#include "stdafx.h"

#include <CE_Camera.h>
#include <CE_DebugDraw.h>
#include <CE_Engine.h>
#include <CE_FileSystem.h>
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

#include <CE_BinaryFileReader.h>
#include <CE_BinaryFileWriter.h>
#include <CE_RendererProxy.h>
#include <CE_TextureManager.h>

LevelEditorContext::LevelEditorContext()
{
}

LevelEditorContext::~LevelEditorContext()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
	CE_SAFE_DELETE(myToolModule);
}

void LevelEditorContext::Init(CE_Engine& anEngine)
{
	myRendererProxy = &anEngine.GetRendererProxy();

	CE_Window& mainWindow = anEngine.GetMainWindow();
	InitGUI(mainWindow.GetUIManager());

	CE_Camera& camera = *mainWindow.GetCamera();
	camera.SetPosition(CE_Vector3f(0.f, 10.f, -7.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.30f));

	myToolModule = new CT_ToolModule(camera, anEngine.GetInput());

	InitWorld(anEngine);
	myEntityFactory = new EntityFactory(*myWorld);

	//OnLoadLevel();
}

void LevelEditorContext::Update(float aDelta)
{
	myToolModule->Update(aDelta);
	myWorld->Update(aDelta);
}

void LevelEditorContext::Render()
{
	//RenderGrid();
	//myToolModule->Render(*myRendererProxy);

	//const CE_Texture* texture = CE_TextureManager::GetInstance()->GetUITexture(CUI_Theme::THEME_BLUE, "button01");
	//myRendererProxy->AddSprite({ 100.f, 200.f }, texture, { 250.f, 125.f });
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
	//CUI_HBox* saveBox = new CUI_HBox();
	//myEditbox = new CUI_EditBox(256.f);
	//CUI_Button* saveButton = new CUI_Button("Save Level");
	//saveButton->myOnClick = std::bind(&LevelEditorContext::OnSaveLevel, this);
	//
	//saveBox->AddWidget(myEditbox);
	//saveBox->AddWidget(saveButton);
	//aUIManager.AddWidget(saveBox);

	CE_GrowingArray<CE_FileSystem::FileInfo> entityFiles;
	CE_FileSystem::GetAllFilesFromDirectory("Data/Entities", entityFiles);

	CUI_Dropbox* dropbox = new CUI_Dropbox("Entities");
	for (const CE_FileSystem::FileInfo& file : entityFiles)
		dropbox->AddLabel(file.myFileNameNoExtention.c_str());

	dropbox->myOnSelection = std::bind(&LevelEditorContext::OnSelection, this, std::placeholders::_1);

	aUIManager.AddWidget(dropbox);
}

void LevelEditorContext::OnSelection(CUI_Widget* aWidget)
{
	CUI_Label* label = static_cast<CUI_Label*>(aWidget);
	const CE_String& text = label->GetText();

	CE_Entity entity = CreateEntity(text);

	EntityInfo& info = myEntityInfos.Add();
	info.myType = text;
	info.myID = entity;
}

void LevelEditorContext::OnSaveLevel()
{
	CE_BinaryFileWriter writer("testLevel.celevel");

	writer.Write(myEntityInfos.Size());
	for (EntityInfo& info : myEntityInfos)
	{
		//TODO: Create EntityInfo for *all* entities, saving/loading has to be able to deal with it
		TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(info.myID);

		int typeLenght = info.myType.Lenght() + 2;
		writer.Write(typeLenght);
		writer.Write(info.myType.c_str(), typeLenght);

		writer.Write(translation.myOrientation.GetPos());
		writer.Write(translation.myScale);
	}
}

void LevelEditorContext::OnLoadLevel()
{
	CE_BinaryFileReader reader("testLevel.celevel");

	int entityCount;
	reader.Read(entityCount);

	myEntityInfos.Reserve(entityCount);
	for (EntityInfo& info : myEntityInfos)
	{
		int typeLength;
		reader.Read(typeLength);

		char typeBuffer[64];
		reader.Read(typeBuffer, typeLength);

		CE_Vector3f position;
		reader.Read(position);
		CE_Vector3f scale;
		reader.Read(scale);
		
		info.myType = typeBuffer;
		info.myID = CreateEntity(info.myType, position, scale);
	}
}

CE_Entity LevelEditorContext::CreateEntity(const CE_String& aType)
{
	CE_Entity entity = myEntityFactory->InstansiateEntity(aType.c_str());
	if (myWorld->HasComponent<TranslationComponent>(entity))
	{
		TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(entity);
		myToolModule->AddToolEntity(entity, &translation.myOrientation, &translation.myScale, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
	}

	return entity;
}

CE_Entity LevelEditorContext::CreateEntity(const CE_String& aType, const CE_Vector3f& aPosition, const CE_Vector3f& aScale)
{
	CE_Entity entity = myEntityFactory->InstansiateEntity(aType.c_str());
	if (myWorld->HasComponent<TranslationComponent>(entity))
	{
		TranslationComponent& translation = myWorld->GetComponent<TranslationComponent>(entity);
		translation.myOrientation.SetPos(aPosition);
		translation.myScale = aScale;

		myToolModule->AddToolEntity(entity, &translation.myOrientation, &translation.myScale, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
	}

	return entity;
}
