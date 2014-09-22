#ifndef TABLE_H_
#define TABLE_H_

#include "gameobj.h"
#include <stdlib.h>
#include <time.h>

class Table : public GameObj {
public:
	Table(SDL_Renderer* renderer);
	~Table();

	void Update(float change);
	void Render(float change);

	float top, bottom;

private:
	SDL_Texture* boundTexture;
};

#endif