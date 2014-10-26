#pragma once

#ifndef GAME_H_
#define GAME_H_

#include "Bullet.h"
#include "GameObject.h"
#include "Player.h"
#include <math.h>
#include <SDL_mixer.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 20
#define MAX_BRICKS 200
#define MAX_ENEMIES 8
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.2f

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


	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	void mapObject(string& type, float plaxeX, float placeY);
	void levelRenderer();

	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float Game::checkPointForGridCollisionX(float x, float y);
	float Game::checkPointForGridCollisionY(float x, float y);
	void Game::doLevelCollisionX(GameObject* gameObj);
	void Game::doLevelCollisionY(GameObject* gameObj);

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
	unsigned int mapWidth;
	unsigned int mapHeight;

	unsigned char** levelData;

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

	Mix_Chunk* gunshot;
	Mix_Chunk* jump;
	Mix_Music* music;

};
#endif
float lerp(float start, float end, float t);