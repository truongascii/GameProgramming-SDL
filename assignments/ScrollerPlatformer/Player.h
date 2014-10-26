#pragma once

#ifndef PLAYER_H_
#define PLAYER_H_
#include "GameObject.h"
#include <SDL_mixer.h>

class Player : public GameObject {
public:
	Player(unsigned int texture);
	void Update(float elapsed);


	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	GLuint chracterTexture;
};
#endif