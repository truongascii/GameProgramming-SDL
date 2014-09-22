#ifndef BALL_H_
#define BALL_H_

#include "gameobj.h"
#include <math.h>

//Speed of the ball
const float BALL_SPEED = 650;

class Ball : public GameObj {
public:
	Ball(SDL_Renderer* renderer);
	~Ball();

	void Update(float change);
	void Render(float change);

	void SetDirection(float dirx, float diry);

	float direction_x, direction_y;

private:
	SDL_Texture* texture;

};
#endif