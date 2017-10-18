#pragma once

#include <CE_Game.h>

class CE_Input;
class CE_Font;
class CE_World;
class CE_Window;

class CUI_Manager;
class CUI_TreeView;
class CUI_Widget;

class EntityFactory;


class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	void InitWorld();
	void InitGrid();

	void InitGUI();
	void PopulateEntityTreeView(unsigned int anEntity);
	void CreatePositionWidget(unsigned int anEntity);
	void CreateMovementWidget(unsigned int anEntity);


	void OnClickFunction();

	CE_World* myWorld;
	CE_World* myTemplateWorld;

	EntityFactory* myEntityFactory;
	CUI_Manager* myUIManager;
	CUI_TreeView* myEntityTreeView;


	CE_Input* myInput;
	CE_Font* myFont;

	CE_Window* myGUIWindow;
};