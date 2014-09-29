#ifndef PLAYER_H_
#define PLAYER_H_

#include "GameObject.h"

class Player : public GameObject {
public:
	Player(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float directionX = 0);
	void Update(float elapsed);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
private:
};
#endif