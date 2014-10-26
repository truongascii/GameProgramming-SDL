#include "Player.h"
using namespace std;

Player::Player(unsigned int texture) :chracterTexture(texture){
	sprite = SpriteSheet(texture, 8, 4, 18);
}

void Player::Update(float elapsed){

	//JUMP
	//if (keys[SDL_SCANCODE_UP]) {
	//	if (!offGround) {
	//		sprite = SpriteSheet(chracterTexture, 8, 4, 20);
	//		collidedBottom = false;
	//		offGround = true;
	//		velocityY = 2.85f;
	//	}

	//}
	//Move right
	if (keys[SDL_SCANCODE_RIGHT]) {
		sprite = SpriteSheet(chracterTexture, 8, 4, 17);
		direction = 1;
		accelerationX = 10.0f * direction;
	}
	//Move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		SpriteSheet playerSprite = SpriteSheet(chracterTexture, 8, 4, 18);
		sprite = playerSprite;
		direction = -1;
		accelerationX = 10.0f * direction;
	}
	//Standing still
	else {
		accelerationX = 0.0f;
	}
}