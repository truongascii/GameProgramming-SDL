#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

/*
Draws a simple scene using texture and colors

Author: Rakin
Date: 9/13/2014

*/

SDL_Window* displayWindow;

using namespace std;
// data structure! Vertex2D type
typedef struct {
	float x;
	float y;
	float r;
	float g;
	float b;
} Vertex2D;

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

//draws the actual image/sprite onto the screen
void DrawSprite(GLint texture, float x, float y, float scale, float rotation, GLenum type, bool colorEnable) {

	//Should we use color 
	(!colorEnable) ? glDisableClientState(GL_COLOR_ARRAY) : glEnableClientState(GL_COLOR_ARRAY); //disable color state, every enable there is a disable

	//Texture binding
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW); 

	//Reset Positions
	glLoadIdentity();

	//Moves the sprite
	glTranslatef(x, y, 0.0);

	//Rotates the sprite
	glRotatef(rotation, 0.0, 0.0, 1.0);

	//Scales the sprite
	glScalef(scale, scale, scale);

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
	glDrawArrays(type, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void loadGame(const char* title, int width, int height){

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//Loading the texture to be used
	GLuint imgTextureSun = LoadTexture("Sun.png");
	GLuint imgTextureGround = LoadTexture("Ground.png");
	GLuint imgTextureBush1 = LoadTexture("Bush1.png");
	GLuint imgTextureBush2 = LoadTexture("Bush2.png");
	GLuint imgTexturePlane = LoadTexture("Plane.png");

	float lastFrameTicks = 0.0f;
	float imgAngles = 0;

	bool done = false;

	SDL_Event event;

	while (!done) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		//setting cyan as color of the background
		glClearColor(0.0f, 0.7f, 0.7f, 1.0f); 

		//Clears the screen to the set bg color.
		glClear(GL_COLOR_BUFFER_BIT);

		//color the triangle
		Vertex2D triangle[3] = { { 0.0, 0.5, 1.0, 0.0, 0.0 },{ -0.5, -0.5, 0.0, 1.0, 0.0 }, 
		{ 0.5, -0.5, 0.0, 0.0, 1.0 } };
		glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle); //stride is 5 as we are skipping 5 to get the values
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0].r);

		//Simple animation
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		imgAngles += elapsed;

		//draws the scene by calling draw sprite section
		//void DrawSprite(texture, x, y, scale, rotation, type, colorEnable) 
		DrawSprite(imgTextureSun, 0.0, 0.8, 0.3, imgAngles*20, GL_QUADS, false); 
		DrawSprite(imgTextureGround, 0.0, 0.0, 2, 0.0, GL_QUADS, false); 
		DrawSprite(NULL, 0.0, -0.125, 0.2, 0.0, GL_TRIANGLES, true); 
		DrawSprite(NULL, -0.2, -0.125, 0.2, 90.0, GL_TRIANGLES, true);
		DrawSprite(NULL, -0.1, -0.375, 0.3, 90.0, GL_QUADS, true);
		DrawSprite(imgTextureBush1, 0.2, -0.45, 0.3, 0.0, GL_QUADS, false);
		DrawSprite(imgTextureBush2, -0.4, -0.45, 0.3, 0.0, GL_QUADS, false);
		DrawSprite(imgTexturePlane, -0.9 + imgAngles*0.2, 0.6, 0.2, 0, GL_QUADS, false);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
}

int main(int argc, char *argv[])
{
	loadGame("Scene Draw", 1280, 720);
	return 0;
}