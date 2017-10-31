#pragma once

#include "GameContext.h"

class CUI_Manager;
class CUI_TreeView;
class CUI_ValueController;
class CUI_HBox;

class CE_Input;
class CE_Font;
class CE_RendererProxy;
class CE_World;

struct RenderComponent;

class EntityEditorContext : public GameContext
{
public:
	EntityEditorContext();
	~EntityEditorContext();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	void InitGUI();
	
	void CreateRenderComponentWidget(RenderComponent& aComponent);
	CUI_TreeView* CreateVectorWidget(const char* aText, CE_Vector3f& aVector);
	CUI_TreeView* CreateColorWidget(const char* aText, CE_Vector4f& aVector);

	CUI_HBox* CreateFloatController(const char* aText, float& aValue);
	void AddModifyButtons(CUI_ValueController* aController, CUI_HBox* aParent);

	void ModifyValueController(CUI_ValueController* aController, float aModifier);

	CUI_Manager* myUIManager;
	CUI_TreeView* myTreeView;

	CE_Input* myInput;
	CE_Font* myFont;
	CE_RendererProxy* myRendererProxy;

	CE_World* myWorld;
};

