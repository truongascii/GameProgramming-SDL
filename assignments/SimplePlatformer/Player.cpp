#include "Player.h"

Player::Player(unsigned int texture) :chracterTexture(texture){
	SpriteSheet playerSprite = SpriteSheet(texture, 8, 4, 18);
	sprite = playerSprite;
}

void Player::Update(float elapsed){

	//JUMP
	if (keys[SDL_SCANCODE_UP]) {
		if (!offGround) {
			SpriteSheet playerSprite = SpriteSheet(chracterTexture, 8, 4, 20);
			sprite = playerSprite;
			collidedBottom = false;
			offGround = true;
			velocityY = 2.85f;
		}

	}
	//Move right
	if (keys[SDL_SCANCODE_RIGHT]) {
		SpriteSheet playerSprite = SpriteSheet(chracterTexture, 8, 4, 17);
		sprite = playerSprite;
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
		velocityX = 0.0f;
		accelerationX = 0.0f;
	}
}