#pragma once
#include "Scene.h"
#include <iostream>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Text.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
class GUI :
	public Scene
{
public:
	GUI();
	~GUI();
	void init();
	void render();
	void update(int deltaTime);
	Scene* changeState();
	void setTime(int time);
	void setDiggers(int digger);
	void setClimbers(int climber);
	void setSpawnRate(int spawnrate);
private:
	void initShader();
	
	int time;
	int out;
	int in;
	int digger;
	int basher;
	int climber;
	int spawnRate;
	ShaderProgram simpleTexProgram;
	Text info;
	glm::mat4 projection;
	Sprite* buttonDigger;
	Sprite* buttonBasher;
	Sprite* buttonClimber;
	Sprite* buttonPlusSpawn;
	Sprite* buttonMinusSpawn;
};

