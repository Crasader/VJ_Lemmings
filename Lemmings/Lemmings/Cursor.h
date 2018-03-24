#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include <iostream>
class Cursor
{
public:
	Cursor();
	~Cursor();
	
	void init();
	void render();
	void update(int x, int y);
private:
	void initShader(); 

	glm::mat4 projection;
	Texture spritesheet;
	ShaderProgram simpleTexProgram;
	Sprite* cursor;
};

