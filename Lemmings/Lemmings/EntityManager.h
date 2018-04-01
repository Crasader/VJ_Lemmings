#pragma once

#include "Lemming.h"
#include <stdio.h>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "VariableTexture.h"
#include "Cursor.h"
#include "DoorEnd.h"
#include "DoorStart.h"


class EntityManager
{
public:
	EntityManager(int numLemmings, glm::vec2 &doorStartPosition, int doorStartType, glm::vec2 &doorEndPosition, int doorEndType, ShaderProgram &shaderProgram,VariableTexture *mask ,string dorIni, string dorEnd);
	~EntityManager();
	void update(int deltaTime, int buttonPressed);
	void render();
	void changeLemmingState(int x);
	bool clickManager(int mouseX, int mouseY, int state);
	bool lemmingInCursor(int mouseX, int mouseY);
	bool checkCollision(glm::vec2 lemmingTopLeftPos, int mouseX, int mouseY);
	void doubleSpeedAnimation();
	void resetNormalSpeed();
	void pause();
	void increaseSpawnTime();
	void decreaseSpawnTime();
	void killAllLemmings();

	int getLemmingsSaved();


private:
	
	bool doubleSpeed;
	bool paused;
	int spawnFrequency;
	vector<Lemming> lemmings;
	Texture spritesheet;
	VariableTexture *mask;
	ShaderProgram shaderProgram;
	glm::vec2 doorStartPosition;
	glm::vec2 doorEndPosition;
	int numLemmings;
	float sceneTime;
	float lastLemmingCreation;
	int buttonPressed;
	DoorEnd* doorEnd;
	DoorStart* doorStart;
	Texture spritesheetStart;
	Texture spritesheetEnd;
	string dorIni;
	string dorEnd;
	bool armagedon;

	int lemmingsSaved;

	int doorSColor;
	int doorEColor;

	void init();	
	void checkStatusLemmings();
};

