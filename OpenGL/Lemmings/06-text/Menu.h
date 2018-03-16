#pragma once
#include "Shader.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "Quad.h"
class Menu
{
public:
	Menu() {}


	static Menu &instance()
	{
		static Menu G;

		return G;
	}


	void render();
	void init();
	void selection();

private:
	
	void initShaders();
	ShaderProgram simpleTexProgram, maskedTexProgram;
	Sprite* titulo;
	Sprite* fondo;
	glm::mat4 projection;
	Texture texturaTitulo, texturaFondo;
	Text playText, exitText;
	bool changeDirection;
	bool exit,play;
	glm::vec4 color1, color2;
	Quad *selector;
};

