#include "stdafx.h"

#include "Game.h"
#include "InGameContext.h"
#include "EntityEditorContext.h"
#include "LevelEditorContext.h"
#include "NodeEditorContext.h"
#include "NeuralNetworkContext.h"
#include "LightingTestContext.h"

Game::Game()
	: myGameContext(nullptr)
{
}


Game::~Game()
{
	CE_SAFE_DELETE(myGameContext);
}

void Game::Init(CE_Engine& anEngine)
{
	//myGameContext = new InGameContext();
	myGameContext = new EntityEditorContext();
	//myGameContext = new LevelEditorContext();
	//myGameContext = new NodeEditorContext();
	//myGameContext = new NeuralNetworkContext();
	//myGameContext = new LightingTestContext();
	myGameContext->Init(anEngine);
}

void Game::Update(float aDelta)
{
	myGameContext->Update(aDelta);
}

void Game::Render()
{
	myGameContext->Render();
}
