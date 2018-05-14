#include "stdafx.h"

#include <Game.h>
#include <CE_Engine.h>

int main()
{
	Game* myGame = new Game();
	CE_Engine myEngine(myGame);

	myEngine.Run();

	delete myGame;
}