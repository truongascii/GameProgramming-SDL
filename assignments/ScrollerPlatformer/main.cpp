#include "Game.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	//start a new game
	Game newGame;
	while (!newGame.UpdateAndRender()) {}
	return 0;
}