#include "Bullet.h"

Bullet::Bullet() {
	xPos = -1.35f;
	yPos = -1.0f;
	
	width = 0.05f;
	height = 0.05f;

	velocityX = 3.0f;
}

void Bullet::Update(float elapsed) {
	xPos += velocityX * elapsed;
}

void Bullet::Render() {
	if (visible) {
		sprite.Draw(width, height, xPos, yPos, rotation);
	}
}