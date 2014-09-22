#include "player.h"

Player::Player(SDL_Renderer* renderr, const char* textName):GameObj(renderr){
	//loads the player texture
	SDL_Surface *surface = IMG_Load(textName);
	texture = SDL_CreateTextureFromSurface(renderr, surface);
	
	width = 32;
	height = 128;

	x = 0;
	y = 0;

	SDL_FreeSurface(surface);
}

Player::~Player(){
	//cleans up resources
	SDL_DestroyTexture(texture);
}

void Player::Update(float change){}

void Player::Render(float change){
	SDL_Rect rectangle;
	rectangle.x = (int)(x + 0.5f);
	rectangle.y = (int)(y + 0.05f);
	rectangle.w = width;
	rectangle.h = height;
	SDL_RenderCopy(renderr, texture, 0, &rectangle);
}