#ifndef GAMEOBJ_H_
#define GAMEOBJ_H_

#include "SDL.h"
#include "SDL_image.h"

class GameObj{
public:
	GameObj(SDL_Renderer* renderr);
	virtual ~GameObj();

	float x, y, width, height;

	virtual void Update(float change);
	virtual void Render(float change);

	bool Collision(GameObj* obj);

protected:
	SDL_Renderer* renderr;
};
#endif