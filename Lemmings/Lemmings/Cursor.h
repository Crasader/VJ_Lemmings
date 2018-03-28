#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include <iostream>
class Cursor
{
public:
	Cursor();
	static Cursor &instance()
	{
		static Cursor G;

		return G;
	}
	~Cursor();
	
	void init();
	void render();
	void update(int x, int y);
	void lemmingInside(bool overLemming);
private:
	void initShader(); 
	void setCursor();
	void changeCursor(bool overLemming);

	glm::mat4 projection;
	Texture spritesheet;
	ShaderProgram simpleTexProgram;
	Sprite* cursor;
	int currentCursorType;
};

