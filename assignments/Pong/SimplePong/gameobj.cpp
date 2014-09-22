#include "gameobj.h"

GameObj::GameObj(SDL_Renderer* renderr){
	this->renderr = renderr;
	x = 0;
	y = 0;
	width = 1;
	height = 1;
}

GameObj::~GameObj(){}

void GameObj::Update(float change){}

void GameObj::Render(float change){}

bool GameObj::Collision(GameObj* obj){
	if (x + width > obj->x && x < obj->x + obj->width &&
		y + height > obj->y && y < obj->y + obj->height){
		return true;
	}
	return false;
}