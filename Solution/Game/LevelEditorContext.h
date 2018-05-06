#pragma once
#include "GameContext.h"

class CE_Font;
class CE_RendererProxy;
class CE_World;

class CT_ToolModule;

class CUI_EditBox;
class CUI_Manager;
class CUI_Widget;

class EntityFactory;

class LevelEditorContext : public GameContext
{
public:
	LevelEditorContext();
	~LevelEditorContext();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	void RenderGrid();

	void InitWorld(CE_Engine& anEngine);

	void InitGUI(CUI_Manager& aUIManager);
	void BuildEntityDropbox(CUI_Manager& aUIManager);

	void OnSelection(CUI_Widget* aWidget);
	void OnSaveLevel();
	void OnLoadLevel();

	CE_Entity CreateEntity(const CE_String& aType);
	CE_Entity CreateEntity(const CE_String& aType, const CE_Vector3f& aPosition, const CE_Vector3f& aScale);

	struct EntityInfo
	{
		CE_Entity myID;
		CE_String myType;
	};
	CE_GrowingArray<EntityInfo> myEntityInfos;

	CE_World* myWorld;
	EntityFactory* myEntityFactory;

	CT_ToolModule* myToolModule;
	CUI_EditBox* myEditbox;

	CE_Font* myFont;
	CE_RendererProxy* myRendererProxy;
};

