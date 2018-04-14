#pragma once

#include "Lemming.h"
#include <stdio.h>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "VariableTexture.h"
#include "Cursor.h"
#include "DoorEnd.h"
#include "DoorStart.h"
#include "Bomb.h"
#include "Firework.h"
#include "Portal.h"


class EntityManager {
public:
	EntityManager(int numLemmings, ShaderProgram &shaderProgram, VariableTexture *map, VariableTexture *mask);
	~EntityManager();
	void init();
	void update(int deltaTime, int buttonPressed,int offsetX, int offsetY);
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
	void pause();
	void increaseSpawnTime();
	void decreaseSpawnTime();
	void killAllLemmings();

	void killAllLemmings2();

	int getLemmingsExited();
	int getLemmingsDied();
	int getBombersAmount();

	void setStartDoor(glm::vec2 doorStartPosition, int doorStartType);
	void setEndDoor(glm::vec2 doorEndPosition, int doorEndType);
	void setBomb(glm::vec2 bombPosition);
	void setPortals(glm::vec2 portalBluePosition, glm::vec2 portalOrangePosition);

	int getNumLemmingsOut();

	void dropBomb(glm::vec2 newPosition);



private:
	
	bool doubleSpeed;
	bool paused;
	int spawnFrequency;
	vector<Lemming*> lemmings;
	Texture spritesheet;
	VariableTexture *mask, *map;
	ShaderProgram shaderProgram;
	glm::vec2 doorStartPosition, doorEndPosition, bombPosition, portalBluePosition, portalOrangePosition;
	int numLemmings;
	float sceneTime;
	float lastLemmingCreation;
	int buttonPressed;
	DoorEnd* doorEnd;
	DoorStart* doorStart;
	Bomb* bomb;
	Portal *portalBlue, *portalOrange;
	Texture spritesheetStart, spritesheetEnd, spritesheetBomb, spritesheetPortals;
	bool armageddon;

	int lemmingsSaved, lemmingsDied;


	int elapsedTime;
	int timeToDisplay;
	Text countdown;
	bool countingDown;
	int offsetX, offsetY;

	int bombersAmount;
	vector<Firework*> fireworks;

	
	void checkStatusLemmings();
	bool lemmingHasActionAssigned(int i, Effect state);
	void startFireworks(int i);
};

