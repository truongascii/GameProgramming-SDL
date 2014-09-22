#ifndef PLAYER_H_
#define PLAYER_H_

#include "gameobj.h"

class Player : public GameObj{
public:
	Player(SDL_Renderer* renderr, const char* textName);
	~Player();

	void Update(float change);
	void Render(float change);
private:
	SDL_Texture* texture;
};
#endif