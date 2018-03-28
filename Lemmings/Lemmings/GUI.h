#pragma once
#include <iostream>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Text.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include "Button.h"
#include "Game.h"
class GUI
{
public:
	GUI();
	~GUI();
	void init();
	void render();
	void update(int mouseX, int mouseY);
	void setTime(int time);
	void setDiggers(int digger);
	void setClimbers(int climber);
	void setSpawnRate(int spawnrate);
private:
	
	int time;
	int out;
	int in;
	int digger;
	int basher;
	int climber;
	int spawnRate;
	int buttonSelected;
	int mouseX;
	int mouseY;
	ShaderProgram simpleTexProgram;
	Text info;
	glm::mat4 projection;
	vector<Button *> buttons;
	void placeButtons();
	void renderButtons();
	void initShader();
	
};

