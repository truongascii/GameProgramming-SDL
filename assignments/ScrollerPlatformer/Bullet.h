#pragma once

#ifndef BULLET_H_
#define BULLET_h_

#include "GameObject.h"
#include "SpriteSheet.h"
class Bullet : public GameObject {
public:
	Bullet();
	void Update(float elapsed);
	void Render();

	float visible;
};
#endif