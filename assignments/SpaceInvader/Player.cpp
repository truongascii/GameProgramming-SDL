#include "Player.h"

Player::Player(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float directionX)
	: GameObject(sprite, scale, x, y, rotation, speed, directionX)	 {}

//player movements
void Player::Update(float elapsed) {	
	if (keys[SDL_SCANCODE_LEFT]) {
		directionX = -speed;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		directionX = speed;
	}
	//boundary check
	if ((!(xPos > 1.21) || directionX < 0) && (!(xPos < -1.21) || directionX > 0))
		xPos += directionX * speed * elapsed;

	directionX = 0;
}