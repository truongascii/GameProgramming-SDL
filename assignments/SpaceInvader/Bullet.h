#pragma once

#ifndef BULLET_H_
#define BULLET_h_

#include "SpriteSheet.h"

class Bullet {
public:
	Bullet();
	Bullet(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed);
	void Update(float elapsed);
	void Render();

	SpriteSheet sprite;

	float scale;
	float x;
	float y;
	int direction;
	float rotation;
	float speed;
	float visible;
};
#endif