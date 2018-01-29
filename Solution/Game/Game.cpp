#include "stdafx.h"

#include "Game.h"
#include "InGameContext.h"
#include "EntityEditorContext.h"

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
	myGameContext = new InGameContext();
	//myGameContext = new EntityEditorContext();
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
