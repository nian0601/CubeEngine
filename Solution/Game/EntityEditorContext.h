#pragma once

#include "GameContext.h"

class CUI_Manager;
class CUI_TreeView;
class CUI_ValueController;
class CUI_HBox;
class CUI_Widget;

class CE_Input;
class CE_Font;
class CE_RendererProxy;
class CE_World;

class CT_ToolModule;

struct RenderComponent;

#include <CUI_Widget.h>

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

	void SetupTestEntities();
	void InitGUI();
	void BuildEntityDropbox();



	void CreateRenderComponentWidget();
	void AddRenderEntry();
	CUI_TreeView* CreateVectorWidget(const char* aText, CE_Vector3f& aVector);
	CUI_TreeView* CreateColorWidget(const char* aText, CE_Vector4f& aVector);

	CUI_HBox* CreateFloatController(const char* aText, float& aValue);
	void AddModifyButtons(CUI_ValueController* aController, CUI_HBox* aParent);

	void ModifyValueController(CUI_ValueController* aController, float aModifier);
	void ClearRenderEntries();

	void OnSelection(CUI_Widget* aWidget);

	RenderComponent* myRenderComponent;

	CUI_Manager* myUIManager;
	CUI_TreeView* myTreeView;
	CUI_TreeView* myRenderComponentView;

	CE_Input* myInput;
	CE_Font* myFont;
	CE_RendererProxy* myRendererProxy;

	CE_World* myWorld;

	int myNumEntries;

	CT_ToolModule* myToolModule;
};

