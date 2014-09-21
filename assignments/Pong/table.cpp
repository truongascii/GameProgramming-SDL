#include "table.h"

Table::Table(SDL_Renderer* renderer) : GameObj(renderer) {
	SDL_Surface* surface = IMG_Load("side.png");
	boundTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	srand(time(0));

	x = 0;
	y = 0;
	width = 800;
	height = 600;

	//Boundaries
	top = 5;
	bottom = 600 - 5;
}

Table::~Table() {
	//Cleans up resources
	SDL_DestroyTexture(boundTexture);
}

void Table::Update(float change) {

}

void Table::Render(float change) {
	//Boundary rendering 
	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = 800;
	rectangle.h = 5;
	SDL_RenderCopy(renderr, boundTexture, 0, &rectangle);

	rectangle.x = 0;
	rectangle.y = 600 - 5;
	rectangle.w = 800;
	rectangle.h = 5;
	SDL_RenderCopy(renderr, boundTexture, 0, &rectangle);
}