#pragma once

#include "Lemming.h"
#include <stdio.h>
#include "ShaderProgram.h"
#include "Sprite.h"


class EntityManager
{
public:
	EntityManager(int numLemmings, glm::vec2 &doorPositonm, ShaderProgram &shaderProgram);
	~EntityManager();
	void update(int deltaTime);
	void render();
private:
	

	vector<Lemming> lemmings;
	Texture spritesheet;
	VariableTexture *mask;
	ShaderProgram shaderProgram;
	glm::vec2 doorPosition;
	void init();
	int numLemmings;
	float sceneTime;
	float lastLemmingCreation;
};

