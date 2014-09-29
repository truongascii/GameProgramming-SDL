#pragma once

#ifndef GAME_H_
#define GAME_H_

#include "Bullet.h"
#include "GameObject.h"
#include "Player.h"
#include <vector>
#include <math.h>
#include <string>

#define MAX_BULLETS 20

using namespace std;

class Game{
public:
	Game();
	~Game();
	GLuint LoadTexture(const char *image_path);

	void shootBullet();
	void Init();
	bool CollisionDetect(const GameObject& enemy, const Bullet& bullet);

	void Update(float elapsed);
	void UpdateMenuScene(float elapsed);
	void UpdateGameScene(float elapsed);
	void UpdateEndScene(float elapsed);

	void Render();
	void RenderMenuScene();
	void RenderGameScene();
	void RenderEndScene();

	bool UpdateAndRender();
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	vector<GameObject*> enemies;

	int bulletIndex;
	Bullet bullets[MAX_BULLETS];

	GLuint spriteSheetTexture;
	GLuint fontSheetTexture;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state;

	int score;
};
#endif