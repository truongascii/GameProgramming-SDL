#include "Game.h"

/*
Rakinur Alam
9/28/2014
*/

int main(int argc, char *argv[])
{ 
	//New Game
	Game app;
	while (!app.UpdateAndRender()) {}
	return 0;
}