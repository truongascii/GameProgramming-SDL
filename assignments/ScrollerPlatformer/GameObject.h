#pragma once

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"

class GameObject {
public:
	GameObject();

	virtual void Update(float elapsed);
	void Render();

	bool collidesWith(GameObject* GameObject);
	void FixedUpdate();

	SpriteSheet sprite;

	float xPos;
	float yPos;
	float width;
	float height;
	float rotation;
	int direction;
	//physics
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	float frictionX;
	float frictionY;
	float mass;
	bool isStatic;

	bool enableCollision;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool offGround;
};

#endif