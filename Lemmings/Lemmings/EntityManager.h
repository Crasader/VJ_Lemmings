#pragma once

#include "Lemming.h"
#include <stdio.h>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "VariableTexture.h"
#include "Cursor.h"


class EntityManager
{
public:
	EntityManager(int numLemmings, glm::vec2 &doorPositonm, ShaderProgram &shaderProgram,VariableTexture *mask);
	~EntityManager();
	void update(int deltaTime);
	void render();
	void changeLemmingState(int x);
	void clickManager(int mouseX, int mouseY, int state);

	bool lemmingInCursor(int mouseX, int mouseY);

	bool checkCollision(glm::vec2 lemmingTopLeftPos, int mouseX, int mouseY);


private:
	

	vector<Lemming> lemmings;
	Texture spritesheet;
	VariableTexture *mask;
	ShaderProgram shaderProgram;
	glm::vec2 doorPosition;
	int numLemmings;
	float sceneTime;
	float lastLemmingCreation;
	int buttonPressed;
	void init();	
};

