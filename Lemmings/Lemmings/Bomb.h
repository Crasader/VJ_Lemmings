#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"
#include "Sprite.h"

class Bomb {
public:
	Bomb();
	~Bomb();
	void init(const glm::vec2 initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *mask);
	void update(int deltaTime);
	void render();
	void changeState();

	enum BombState {
		IDLE_STATE,
		PICKED_STATE,
		DROPPED_STATE,
		EXPLODING_STATE,
		EXPLODED_STATE,
		END_STATE
	};
	BombState getState();

	void setPosition(glm::vec2 newPosition);
	glm::vec2 getPosition();

	void goPicked();
	void goDrop();
	void goTrigger();
	void goExplode();
	void goEnd();

	void doubleSpeed();
	void resetSpeed();
	void pause();

private:
	BombState state;
	float currentTime;

	void explode();
	void deleteMask(int xStart, int xEnd, int y);
	void setMask(VariableTexture *mapMask);
	void setAnimations();
	void resetActionTime();

	enum BombAnims { // 4 animations
		IDLE,
		DROPPED,
		GOING_TO_EXPLODE,
		EXPLODING,
	};

	Sprite *sprite;
	VariableTexture *mask;


	int actionTime;

};

