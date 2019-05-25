#pragma once

#include "GameContext.h"

class CUI_Manager;
class CUI_TreeView;
class CUI_ValueController;
class CUI_HBox;
class CUI_Widget;
class CUI_EditBox;

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

	void InitGUI();

	void CreateRenderComponentWidget();
	void AddRenderEntry();
	CUI_TreeView* CreateVectorWidget(const char* aText, CE_Vector3f& aVector);
	CUI_TreeView* CreateColorWidget(const char* aText, CE_Vector4f& aVector);

	CUI_HBox* CreateFloatController(const char* aText, float& aValue);
	void ClearRenderEntries();

	RenderComponent* myRenderComponent;

	CUI_Manager* myUIManager;
	CUI_TreeView* myTreeView;
	CUI_TreeView* myRenderComponentView;

	CE_Input* myInput;
	CE_RendererProxy* myRendererProxy;

	CE_World* myWorld;
	EntityFactory* myEntityFactory;

	int myNumEntries;
};

