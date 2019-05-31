#pragma once

#include "GameContext.h"

class CUI_Dropbox;
class CUI_Manager;
class CUI_Widget;

class CE_Input;
class CE_RendererProxy;
class CE_World;

class CT_ToolModule;

struct RenderComponent;

class EntityFactory;

class EntityEditorContext : public GameContext
{
public:
	EntityEditorContext();
	~EntityEditorContext();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	void RenderGrid();

	void SetupConstantWidgets();
	void OnModelDropboxSelection(CUI_Widget* aWidget, int aWidgetIndex);

	CE_Entity myEntity;
	RenderComponent* myRenderComponent;

	CUI_Manager* myUIManager;

	CE_Input* myInput;
	CE_RendererProxy* myRendererProxy;

	CE_World* myWorld;
	EntityFactory* myEntityFactory;

	CUI_Dropbox* myModelsDropbox;

	CT_ToolModule* myToolModule;
};

