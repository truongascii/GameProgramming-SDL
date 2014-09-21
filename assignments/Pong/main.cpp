#include "game.h"

int main(int argc, char* argv[]) {

	//Starting new game
	Game* game = new Game();

	//Initialization and runs
	if (game->Init()) {
		game->Run();
	}

	//clean up
	delete game;
	return 0;
}