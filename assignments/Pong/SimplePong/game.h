#ifndef GAME_H_
#define GAME_H_

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

#include "table.h"
#include "player.h"
#include "ball.h"

#define FPS_DELAY 500
#define AI_SPEED 400

class Game {
public:
	Game();
	~Game();

	bool Init();
	void Run();

private:
	SDL_Window* displayWindow;
	SDL_Renderer* renderr;

	unsigned int lasttick, fpstick, fps, framecount;

	Table* table;
	std::vector<Player*> players;
	Ball* ball;

	void Clean();
	void Update(float change);
	void Render(float change);

	void NewGame();

	void SetPlayerY(int index, float y);
	void CheckCollision();
	float GetReflection(int index, float hit);

	void AI(float change);
};

#endif
