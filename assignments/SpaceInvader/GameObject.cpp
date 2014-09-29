#include "GameObject.h"

//constructor
GameObject::GameObject(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float directionX, float directionY)
	: sprite(sprite), scale(scale), xPos(x), yPos(y), rotation(rotation), speed(speed), directionX(directionX), directionY(directionY) {}

//getters 
float GameObject::getX() { 
	return xPos; 
}
float GameObject::getY() { 
	return yPos; 
}

//updates the game object
void GameObject::Update(float elapsed) {
	//boundary check
	if ((!(xPos > 1.2) || directionX < 0) && (!(xPos < -1.2) || directionX > 0)) {
		xPos += directionX * speed * elapsed * 5;
		yPos += directionY * speed * elapsed * 3;
	}	
}

//renders the game object
void GameObject::Render() {
	sprite.Draw(scale, xPos, yPos, rotation);
}