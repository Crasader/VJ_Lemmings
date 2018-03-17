#pragma once
#include "Shader.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "Quad.h"
#include "Scene.h"
#include <GL/glut.h>
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
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void update(int deltaTime);
	void keyPressed(int key);
	void specialKey(int key);

private:
	
	void initShaders();
	void enter();
	void selection();
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
	bool scene;
	Scene sceneObj;
};

