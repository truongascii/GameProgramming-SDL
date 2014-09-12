#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

/*
covers how to draw a triable on screen
how to move rotate and scale the triangle
*/

void loadGame(const char* title, int width, int height){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	
	//Sets the pixel size and offset of rendering area. *starting bottom left (0,0)
	//void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
	glViewport(0, 0, 800, 600); 
	//glMatrixMode(matrixMode); GL_PROJECTION = projection matrix, GL_MODELVIEW = modelview matrix 
	glMatrixMode(GL_PROJECTION); 
	//void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	//multiplies the current matrix with orthographic projection matrix
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);


	bool done = false;

	SDL_Event event;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		//Operation on the shapes *transformation, *rotation, *scale
		//void glLoadIdentity(); resets the matrix back to original
		//replaces current matrix with identity matrix

		//void glTranslatef (GLfloat x, GLfloat y, GLfloat z); moving the share (multiplying the current matrix by translation matrix)
		glLoadIdentity(); //reset
		glTranslatef(0.5, 0.5, 0.0); //move right 0.5(x) up 0.5(y)

		//void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z); roatating the shape (multiply the current matrix by a rotation matrix)
		glRotatef(45.0f, 0.0, 0.0, 1.0); //rotate 45 degree

		//void glScalef (GLfloat x, GLfloat y, GLfloat z); change the size of a shape *scale up or down. (multiply current matrix by scale matrix)
		glScalef(2, 1.0, 1.0); //change the size make it bigger, <1 value smaller >1 bigger 1 = no change

		//glVertexPointer (GLint size, GLenum type, GLsizei stride (how many to skip), const GLvoid *pointer);
		//Defines an array of vertex data *we are drawing a triable
		GLfloat triangle[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		//glEnableClientState(GLenum array); enabels client state capability (for every enable there is a disable)
		glEnableClientState(GL_VERTEX_ARRAY);
		//glDrawArrays (GLenum mode, GLint first, GLsizei count);
		//draws/renders the previously defined "shape" array onto the screen
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();

}
int main(int argc, char *argv[])
{
	loadGame("MyGame", 800, 600);
	return 0;
}