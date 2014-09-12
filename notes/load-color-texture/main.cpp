#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

/*
- Color implemetation 
- Loading a texture & assigning it
- Blending
- Drawing squares!! yay
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

	if (surface == NULL)
	{
		//finds the current working directory
		char directory[1024];
		GetCurrentDirectory(1024, directory);
		OutputDebugString(directory); //finds the current working directory
		
		OutputDebugString(IMG_GetError()); //what went wrong with the image
		OutputDebugString("error loading the image!\n");
		exit(2);
	}

	GLuint textureID;

	//void glGenTextures (GLsizei n, GLuint *textures); Genarates new openGL texture id
	glGenTextures(1, &textureID);

	//void glBindTexture (GLenum target, GLuint texture); bind texture to a target
	glBindTexture(GL_TEXTURE_2D, textureID);

	//void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	//Sets texture data to the specified texture target
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	//Image format must be GL_RGBA or GL_RGB for RGB images. image may also load as GL_BGRA or GL_BGR format

	//void glTexParameteri (GLenum target, GLenum pname, GLint param);
	//set texture parameter of specified texture target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//GL_LINEAR used for smooth & GL_NEAREST used for pixalated graphics

	//no longer needed so free it up for optimization
	SDL_FreeSurface(surface);
	return textureID;
}

//draws the actual image/sprite onto the screen
void DrawSprite(GLint texture, float x, float y, float rotation, GLenum type) {

	glEnable(GL_TEXTURE_2D);
	//void glBindTexture (GLenum target, GLuint texture); bind texture to a target
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW); //sets up model view matrix

	//void glLoadIdentity(); resets the matrix back to original
	glLoadIdentity();

	//Moves the target x, y, z(0) direction
	glTranslatef(x, y, 0.0);

	//Rotates the target according to rotation value
	glRotatef(rotation, 0.0, 0.0, 1.0);

	//glDrawArrays (GLenum mode, GLint first, GLsizei count);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 }; //QUADS are predefined so we can just create it by passing in GL_QUADS

	//void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	//allocates arry of vertexture data coordinates (UV) data.
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs); //inthis case the quad's UV

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//allows blending to happen, transparency 
	glEnable(GL_BLEND);

	/*
	SRC: GL_SRC_ALPHA
	DST: GL_ONE
	Creates an additive blend, which looks spectacular on a black background 
	but terrible on brighter colours, as it begines to fade to white.

	SRC: GL_SRC_ALPHA
	DST: GL_ONE_MINUS_SRC_ALPHA
	This allows other backgrounds to be used without affecting the color of the particles, 
	but the particles themselves look dull compared to the additive blend.
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(type, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void loadGame(const char* title, int width, int height){

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	GLuint imgTexture = LoadTexture("smiley.png");
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

		//void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha); the color of the screen 
		// we use [RGBA] format [A]lpha is for blending/transparency. sometime might be inverse [BGRA]
		glClearColor(0.4f, 0.2f, 0.4f, 1.0f); //0.4Red, 0.2Green, 0.4Blue, 0.1Alpha (not transparent) = Color-> light purple

		//void glClear (GLbitfield mask);  Clears the screen to the set clear color.
		glClear(GL_COLOR_BUFFER_BIT);

		glTranslatef(0.65, 0.65, 0.0); //move right 0.5(x) up 0.5(y)

		//Drawining a triangle
		/*GLfloat triangle[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);*/

		//void glColorPointer (GLint size, GLenum type, GLsizei stride (how many to skip), const GLvoid *pointer); Defines an array of vertex color data.
		GLfloat triangleColors[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
		glColorPointer(3, GL_FLOAT, 0, triangleColors);
		glEnableClientState(GL_COLOR_ARRAY);

		//better way to color the triangle
		Vertex2D triangle[3] = { { 0.0, 0.5, 1.0, 0.0, 0.0 },{ -0.5, -0.5, 0.0, 1.0, 0.0 }, 
		{ 0.5, -0.5, 0.0, 0.0, 1.0 } };
		glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle); //stride is 5 as we are skipping 5 to get the values
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0].r);
		glEnableClientState(GL_COLOR_ARRAY); //enables color state, every enable there is a disable
		glDrawArrays(GL_TRIANGLES, 0, 3); // draws the predifined triangle 

		glDisableClientState(GL_COLOR_ARRAY); //disable color state, every enable there is a disable

		//Simple animation
		float ticks = (float)SDL_GetTicks() / 1000.0f; //getTicks returns in ms so a large number device by 1000 to make it stable
		float elapsed = ticks - lastFrameTicks; //
		lastFrameTicks = ticks;

		imgAngles += elapsed;
		//draws the actual spriate with the texture, angle rotation speed, and type of body *quads in this case
		DrawSprite(imgTexture, 0.0, 0.0, imgAngles*50, GL_QUADS); //rotates the iamge without changing position.

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();

}
int main(int argc, char *argv[])
{
	loadGame("MyGame", 800, 600);
	return 0;
}