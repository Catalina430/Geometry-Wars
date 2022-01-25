#include <iostream>

#include "Common.h"

#include "Game.h"

//Geometry Wars clone for COMP4300 assignment #2 Using ECS and factory design pattern.
//Also data driven ie coonfig file

int main(int argc, char * argv[])
{
	//Constructs the game with a config file
	//On the heap
	Game* game = new Game("src/Config.txt");
	//Run the game
	game->run();

	delete game;
	return 0;
}