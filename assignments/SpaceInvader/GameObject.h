#pragma once

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "SpriteSheet.h"

class GameObject {
public:
	GameObject(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float directionX = 0, float directionY = 0);

	float getX();
	float getY();
	//virtual allows player to override the update function
	virtual void Update(float elapsed);
	void Render();

	SpriteSheet sprite;

	float scale;
	float xPos;
	float yPos;
	float rotation;

	float speed;
	float directionX;
	float directionY;
};
#endif