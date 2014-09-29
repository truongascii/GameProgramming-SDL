#include "Bullet.h"

//Default constructor
Bullet::Bullet() {
	x = -1.35f;
	y = -1.0f;
}
//Constructor
Bullet::Bullet(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed)
	: sprite(sprite), scale(scale), x(x), y(y), rotation(rotation), speed(speed) {}
//Updates the bullet
void Bullet::Update(float elapsed) {
	y += speed * elapsed;
}
//Renders the bullet
void Bullet::Render() {
	if (visible) {
		sprite.Draw(scale, x, y, rotation);
	}
}