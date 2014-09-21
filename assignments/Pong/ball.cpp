#include "ball.h"

Ball::Ball(SDL_Renderer* renderer) : GameObj(renderer) {
	SDL_Surface* surface = IMG_Load("ballGrey.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	x = 0;
	y = 0;
	width = 24;
	height = 24;

	SetDirection(1, 1);
	SDL_FreeSurface(surface);
}

Ball::~Ball() {
	//cleans up resources
	SDL_DestroyTexture(texture);
}

void Ball::Update(float change) {
	//ball movement
	x += direction_x * change;
	y += direction_y * change;
}

void Ball::Render(float change) {
	SDL_Rect rect;
	rect.x = (int)(x + 0.5f);
	rect.y = (int)(y + 0.5f); 
	rect.w = width;
	rect.h = height;
	SDL_RenderCopy(renderr, texture, 0, &rect);
}

void Ball::SetDirection(float dirx, float diry) {
	//set direction of the ball 
	float length = sqrt(dirx * dirx + diry * diry);
	this->direction_x = BALL_SPEED * (dirx / length);
	this->direction_y = BALL_SPEED * (diry / length);
}