#include <iostream>

#include "Common.h"

#include "Game.h"

int main(int argc, char * argv[])
{
	//Constructs the game with a config file
	Game game("src/Config.txt");
	//Run the game
	game.run();

	return 0;
}