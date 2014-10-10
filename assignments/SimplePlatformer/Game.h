#pragma once

#ifndef GAME_H_
#define GAME_H_

#include <math.h>
#include "Bullet.h"
#include "GameObject.h"
#include "Player.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 20
#define MAX_BRICKS 50
#define MAX_ENEMIES 8

using namespace std;

class Game{
public:
	Game();
	~Game();

	void Init();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
	bool UpdateAndRender();

	void buildLevel();
	void shootBullet();
	

private:
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	Player* player;
	vector<GameObject*> gameObjs;

	float gravityX;
	float gravityY;

	int bulletIndex;
	int brickIndex;
	int enemyIndex;

	Bullet bullets[MAX_BULLETS];
	GameObject bricks[MAX_BRICKS];
	GameObject enemies[MAX_ENEMIES];

	GLuint chracterTexture;
	GLuint levelTexture;
	GLuint fontSheetTexture;

	float shootTimer;
	float enemySpawnTimer;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state;

	int score;
	int life;

};
#endif
float lerp(float start, float end, float t);