#pragma once

#include "GameContext.h"

class CUI_Manager;

class CE_Input;
class CE_Font;
class CE_RendererProxy;

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
	void PopulateEntityTreeView(unsigned int anEntity);
	void CreatePositionWidget(unsigned int anEntity);
	void CreateMovementWidget(unsigned int anEntity);

	void OnClickFunction();


	CUI_Manager* myUIManager;


	CE_Input* myInput;
	CE_Font* myFont;
	CE_RendererProxy* myRendererProxy;
};

