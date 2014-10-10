#pragma once

#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>

using namespace std;

GLuint LoadTexture(const char *image_path);
void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);

class SpriteSheet{
public:
	SpriteSheet();
	SpriteSheet(GLuint textureID, unsigned int spriteCountX, unsigned int spriteCountY, unsigned int index);

	void Draw(float width, float height, float x, float y, float rotation);

	GLuint textureID;
	unsigned int spriteCountX;
	unsigned int spriteCountY;
	unsigned int index;
};
#endif