#include "GameObject.h"

//initialize all the variables
GameObject::GameObject() {
	xPos = 0.0f;
	yPos = 0.0f;
	width = 0.2f;
	height = 0.2f;
	rotation = 0.0f;
	direction = 1;

	velocityX = 0.0f;
	velocityY = 0.0f;
	accelerationX = 0.0f;
	accelerationY = 0.0f;
	frictionX = 0.0f;
	frictionY = 0.0f;
	mass = 0.0f;

	isStatic = false;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	offGround = false;
}

void GameObject::Update(float elapsed) {	
}

void GameObject::Render() {
	sprite.Draw(width, height, xPos, yPos, rotation);
}

//check for collision with other game objects
bool GameObject::collidesWith(GameObject* GameObject) {
	float Top = yPos + height / 2;
	float Bot = yPos - height / 2;
	float Right = xPos + width / 2;
	float Left = xPos - width / 2;

	float objTop = GameObject->yPos + GameObject->height / 2;
	float objBot = GameObject->yPos - GameObject->height / 2;
	float objRight = GameObject->xPos + GameObject->width / 2;
	float objLeft = GameObject->xPos - GameObject->width / 2;

	if (Bot > objTop)
		return false;
	if (Top < objBot)
		return false;
	if (Left > objRight)
		return false;
	if (Right < objLeft)
		return false;
		
	return true;
}
void GameObject::FixedUpdate() {

}