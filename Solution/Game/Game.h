#pragma once

#include <CE_Game.h>

class CE_World;
class EntityFactory;
class CUI_Manager;
class CUI_Widget;
class CE_Input;

class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render(CE_RendererProxy& anRendererProxy) override;

private:
	void InitWorld();
	void InitGrid();

	void InitGUI();

	void OnClickFunction();

	CE_World* myWorld;
	CE_World* myTemplateWorld;

	EntityFactory* myEntityFactory;
	CUI_Manager* myUIManager;
	CUI_Widget* myWidget;

	CE_Input* myInput;
};