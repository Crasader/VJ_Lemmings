#pragma once

#include "Lemming.h"
#include <stdio.h>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "VariableTexture.h"
#include "Cursor.h"
#include "DoorEnd.h"
#include "DoorStart.h"


class EntityManager {
public:
	EntityManager(int numLemmings, glm::vec2 &doorStartPosition, int doorStartType, glm::vec2 &doorEndPosition, int doorEndType, ShaderProgram &shaderProgram, VariableTexture *map, VariableTexture *mask ,string dorIni, string dorEnd);
	~EntityManager();
	void update(int deltaTime, int buttonPressed);
	void render();


	enum Effect {
		BASHER_EFFECT,
		BLOCKER_EFFECT,
		CLIMBER_EFFECT,
		DIGGER_EFFECT,
		FLOATER_EFFECT,
		BOMBER_EFFECT,
		BUILDER_EFFECT,
		ARMAGEDDON_EFFECT,
		NONE_EFFECT
	};

	void changeLemmingState(int x);
	bool clickManager(int mouseX, int mouseY, Effect state);
	bool lemmingInCursor(int mouseX, int mouseY);
	bool checkCollision(glm::vec2 lemmingTopLeftPos, int mouseX, int mouseY);
	void doubleSpeedAnimation();
	void resetNormalSpeed();
	void pause();
	void increaseSpawnTime();
	void decreaseSpawnTime();
	void killAllLemmings();

	int getLemmingsExited();
	int getLemmingsDied();



private:
	
	bool doubleSpeed;
	bool paused;
	int spawnFrequency;
	vector<Lemming*> lemmings;
	Texture spritesheet;
	VariableTexture *mask, *map;
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
	bool armageddon;

	int lemmingsSaved, lemmingsDied;

	int doorSColor;
	int doorEColor;

	

	void init();	
	void checkStatusLemmings();
	bool lemmingHasActionAssigned(int i, Effect state);
};

