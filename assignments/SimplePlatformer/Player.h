#pragma once

#ifndef PLAYER_H_
#define PLAYER_H_
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(unsigned int texture);
	void Update(float elapsed);

	void jump();
	void move();
	void reset();

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	GLuint chracterTexture;
};
#endif