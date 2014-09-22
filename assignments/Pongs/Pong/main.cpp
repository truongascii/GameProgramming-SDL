#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265359
#define BALL_SPEED 0.5

int p1score = 0;
int p2score = 0;

SDL_Window* displayWindow;

using namespace std;

class GameObjects{
public:
	GameObjects(float x, float y, float r, int tid, float w, float h, float s){
		xPos = x;
		yPos = y;
		rotation = r;

		textureID = tid;

		width = w;
		height = h;

		speed = s;
		//controlUp = up;
		//controlDown = down;
	}

	bool CollideWith(GameObjects* obj){

		if (obj->xPos - obj->height < xPos && xPos < obj->xPos + obj->height &&
			obj->yPos - obj->width < yPos && yPos < obj->yPos + obj->width) {
			return true;
		}
		return false;
	}

	void SetDirection(float dirx, float diry) {
		//set direction of the ball 
		directionX = BALL_SPEED * (dirx);
		directionY = BALL_SPEED * (diry);
	}

	void Draw(){
		//Texture binding
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		glTranslatef(xPos, yPos, 0);

		glScalef(height, width, 0);

		glRotatef(rotation, 0, 0, 1);

		GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

		//allocates arry of vertexture data coordinates (UV) data.
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Drawing
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	
	}

	float xPos;
	float yPos;
	float rotation;

	float width;
	float height;

	float speed;
	float directionX = BALL_SPEED;
	float directionY = -BALL_SPEED;

private:

	int textureID;

};

//function to load texture into the game
GLuint LoadTexture(const char *image_path) {

	//loads the image using given path
	SDL_Surface *surface = IMG_Load(image_path);

	//If unable to load texture
	if (surface == NULL)
	{
		char directory[1024];
		GetCurrentDirectory(1024, directory);
		OutputDebugString(directory); //Checking Working directory

		OutputDebugString(IMG_GetError()); //What went wrong
		OutputDebugString("error loading the image!\n");
		exit(2);
	}

	GLuint textureID;

	//Genarates new openGL texture id
	glGenTextures(1, &textureID);

	//void glBindTexture (GLenum target, GLuint texture); bind texture to a target
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Sets texture data to the specified texture target
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	//set texture parameter of specified texture target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeup surface
	SDL_FreeSurface(surface);
	return textureID;
}

void SetupGame(const char* title, int width, int height){

	//to initialize joystick
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);	

}

float lastFrameTicks = 0.0f;
void Update(GameObjects* p1, GameObjects* p2, GameObjects* ball){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float moveX = cos(ball->rotation * (PI / 180.0));
	float moveY = sin(ball->rotation * (PI / 180.0));

	ball->xPos += ball->directionX * elapsed;
	ball->yPos += ball->directionY * elapsed;

	if (ball->CollideWith(p2)){

		ball->directionX = BALL_SPEED * moveX;
		//ball->directionY = BALL_SPEED * moveY;
		ball->xPos = p2->xPos + p2->height;
	}

	else if(ball->CollideWith(p1)){

		ball->directionX = -BALL_SPEED * moveX;
		//ball->directionY = BALL_SPEED * moveY;

		ball->xPos = p1->xPos - p1->height;
	}
	//p2 score
	else if (ball->xPos > 1.33){
		p2score++;
		ball->xPos = 0;
		ball->yPos = 0;
		if (p2score == 5){
			OutputDebugString("Player 2 Wins\n");
			exit(1);
		}
	}
	//p1 score
	else if (ball->xPos < -1.33){
		p1score++;
		ball->xPos = 0;
		ball->yPos = 0;	
		if (p2score == 5){
			OutputDebugString("Player 1 Wins\n");
			exit(1);
		}
	}
	else if (ball->yPos > 1 || ball->yPos < -1){
		ball->directionY *= -1;
	}

}

void PlayerControll(GameObjects* p1, GameObjects* p2, SDL_Event event){

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_UP){
			if (p1->yPos > 0.85){
				p1->yPos = 0.85;
			}
			p1->yPos += 0.0005;
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
			if (p1->yPos < -0.85){
				p1->yPos = -0.85;
			}
			p1->yPos -= 0.0005;
		}

		if (event.key.keysym.scancode == SDL_SCANCODE_UP){
			if (p2->yPos > 0.85){
				p2->yPos = 0.85;
			}
			p2->yPos += 0.0005;
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
			if (p2->yPos < -0.85){
				p2->yPos = -0.85;
			}
			p2->yPos -= 0.0005;
		}
	}
}

void Render(GameObjects* p1, GameObjects* p2, GameObjects* ball){

	//setting cyan as color of the background
	glClearColor(0.0f, 0.7f, 0.7f, 1.0f);
	//Clears the screen to the set bg color.
	glClear(GL_COLOR_BUFFER_BIT);

	p1->Draw();
	p2->Draw();
	ball->Draw();
}

void Cleanup(){

	// Destroy window
	SDL_DestroyWindow(displayWindow);

	SDL_Quit();
}

int main(int argc, char *argv[])
{
	SetupGame("Scene Draw", 800, 600);
	//Loading the texture to be used
	GLuint imgTextureP1 = LoadTexture("paddleBlu.png");
	GLuint imgTextureP2 = LoadTexture("paddleRed.png");
	GLuint imgTextureBall = LoadTexture("ballGrey.png");

	GameObjects p1(0.9, 0, 90, imgTextureP1, 0.3, 0.05, 1.0);
	GameObjects p2(-0.9, 0, 90, imgTextureP2, 0.3, 0.05, 1.0);
	GameObjects ball(0, 0, 45, imgTextureBall, 0.07, 0.07, 1.0);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	bool done = false;

	SDL_Event event;

	while (!done) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		Render(&p1, &p2, &ball);
		PlayerControll(&p1, &p2, event);
		Update(&p1, &p2, &ball);

		SDL_GL_SwapWindow(displayWindow);
	}
	Cleanup();
	return 0;
}