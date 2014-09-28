
#include <SDL.h> //standard SDL library
#include <SDL_opengl.h> // opengl library api for graphics across past 30 days, guranteed to run on cross platform
#include <SDL_image.h> // library to load images for all of our games

#include <vector>
#include <string>

using namespace std;

SDL_Window* displayWindow;

//what if our texture is larger or smaller than our target
//repeat and plant (starts to tile: tiling)
//clamp will take last pixle and keep going with it (would want to use  in some cases)
//use glTexParametri for this
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
	//since we are using pixel art we do gl_nearest
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//S(u) = (x) WIDTH AND T(v) = HEIGHT(y): we using GL_REPEAT for tiling
	//can use clamp to clamp to a corner
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Freeup surface
	SDL_FreeSurface(surface);
	return textureID;
}


//loading a sprite from sprite sheet
void DrawSpriteSheetSprite(int spriteTexture, int index, int spriteCountX, int spriteCountY){
	//how many in x and how many in y
	glBindTexture(GL_TEXTURE_2D, spriteTexture);
	glEnable(GL_TEXTURE_2D);

	//setting u/s/x and v/t/y
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteW = 1.0 / (float)spriteCountX;
	float spriteH = 1.0 / (float)spriteCountY;

	GLfloat quadsUV[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quadsUV);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat uvs[] = { u, v, u, v + spriteH, u + spriteW, v + spriteH, u + spriteW, v };
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//disable since not using colors for sprites
	glDisableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);
}



//FONTS
//ASCII valuess are used for  past 20 years, now we have unicode
//font seets are usually ordered in ascii order
//print the index using the ascii values
void DrawTexts(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
}


void loadGame(const char* title, int width, int height){
	SDL_Init(SDL_INIT_VIDEO); //things you pass in must start with this 

	displayWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//blending is slow, so just use it for stuff that needs blending 
	//(unless there is small amount of sprites)
	glEnable(GL_BLEND);

	const int runAnimation[] = { 9, 10, 11, 12, 13 };
	const int numFrames = 5;
	float animationElapsed = 0.0f;
	float framesPerSeconds = 30.0f;

	GLuint spriteTestTexture = LoadTexture("characters_3.png");
	GLuint fontTexture = LoadTexture("font1.png");
	GLuint tileTexture = LoadTexture("tile.png");

	float lastFrameTicks = 0.0f;

	bool done = false;

	int currentIndex = 0;

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, 1, 1);
	glMatrixMode(GL_MODELVIEW);

	float bgScroll = 0.0f;

	SDL_Event event;
	//keeps the window running through loop
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		animationElapsed += elapsed;
		//animating using sprite sheet
		if (animationElapsed > 1.0 / framesPerSeconds){
			currentIndex++;
			animationElapsed = 0.0f;
			if (currentIndex > numFrames - 1){
				currentIndex = 0;
				animationElapsed += elapsed;
			}
		}

		bgScroll += elapsed * 0.5;

		DrawSpriteSheetSprite(spriteTestTexture, runAnimation[currentIndex], 8, 4);
		DrawTexts(fontTexture, "Helloworld", 0.1, 0, 1.0, 1.0, 1.0, 1.0);
		SDL_GL_SwapWindow(displayWindow);
		
	}
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	loadGame("Loading Window", 800, 600);
	return 0;
}