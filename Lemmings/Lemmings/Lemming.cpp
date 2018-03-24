#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4




Lemming::Lemming()
{
	
}

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *mask)
{
	setMapMask(mask);
	state = FALLING_RIGHT_STATE;

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.03125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(19);

	setAnimations();

	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}



void Lemming::update(int deltaTime) {
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch(state)	{
		case FALLING_LEFT_STATE:
			fall = collisionFloor(2);
			if (fall > 0) {		// more falling
				move(0, fall);
			}
			else {				// no more falling
				if (nextState == DIGGER_TRIGGERED) {
					oldState = state;
					state = DIGGER_TRIGGERED;
				}
				else {
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
				}
			}
			break;


		case FALLING_RIGHT_STATE:
			fall = collisionFloor(2);
			if (fall > 0) {		// more falling
				move(0, fall);
			}
			else {				// no more falling
				if (nextState == DIGGER_TRIGGERED) {
					oldState = state;
					state = DIGGER_TRIGGERED;
				}
				else {
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
				}
			}
			break;


		case WALKING_LEFT_STATE:
			move(-1, -1);
			if(collision()) {	// wall
				move(1, 1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
			else {				// no wall
				fall = collisionFloor(3);
				if (fall < 3)
					move(0, fall);
				else {
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
			}
			break;


		case WALKING_RIGHT_STATE:
			move(1, -1);
			if(collision()) {	// wall
				move(-1, 1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
			else {				// no wall
				fall = collisionFloor(3);
				if (fall < 3) 
					move(0, fall);
				else {
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
				}
					
			}
			break;


		case DIGGER_TRIGGERED:
			fall = collisionFloor(2);
			if (fall > 0) {
				if (oldState == FALLING_LEFT_STATE) {
					state = FALLING_LEFT_STATE;
					move(0, 1);
				}
				else {
					state = FALLING_RIGHT_STATE;
					move(0, 1);
				}
				nextState = DIGGER_TRIGGERED;
			}
			else {
				move(0, 1);
				sprite->changeAnimation(DIGGER);
				state = DIGGER_STATE;
			}
			break;


		case DIGGER_STATE:
			fall = collisionFloor(2);
			if (fall > 0) {
				if (oldState == FALLING_LEFT_STATE) {
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
				else {
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
				}
				move(0, 0.125f);
			}
			else {
				move(0, 0.125f);
			}
			break;

		default:
			break;
	}
}

void Lemming::render() {
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask) {
	mask = mapMask;
}

glm::vec2 Lemming::getPosition() {
	return sprite->position();
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(0, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(0, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::move(float x, float y) {
	sprite->position() += glm::vec2(x, y);
}

void Lemming::changeState(int x) {
	if (x == 1) {
		oldState = state;
		state = DIGGER_TRIGGERED;
	}

}

void Lemming::setAnimations() {

	/* Walking */
		sprite->setAnimationSpeed(WALKING_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0));

		sprite->setAnimationSpeed(WALKING_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.f / 32));

	/* Falling */
		sprite->setAnimationSpeed(FALLING_RIGHT, speed);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 2.f / 32));

		sprite->setAnimationSpeed(FALLING_LEFT, speed);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 8, 2.f / 32));

	/* Blocking */
		sprite->setAnimationSpeed(BLOCKER, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BLOCKER, glm::vec2(float(i) / 8, 3.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BLOCKER, glm::vec2(float(i) / 8, 4.f / 32));

	/* Digging */
		sprite->setAnimationSpeed(DIGGER, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIGGER, glm::vec2(float(i) / 8, 5.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIGGER, glm::vec2(float(i) / 8, 6.f / 32));

	/* Bashing */
		sprite->setAnimationSpeed(BASHER_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 7.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 8.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 9.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 10.f / 32));

		sprite->setAnimationSpeed(BASHER_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 11.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 12.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 13.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_RIGHT, glm::vec2(float(i) / 8, 14.f / 32));

	/* Climbing */
		sprite->setAnimationSpeed(CLIMBER_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(CLIMBER_RIGHT, glm::vec2(float(i) / 8, 15.f / 32));

		sprite->setAnimationSpeed(CLIMBER_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(CLIMBER_LEFT, glm::vec2(float(i) / 8, 16.f / 32));

	/* Climbing top */
		sprite->setAnimationSpeed(CLIMBER_TOP_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(CLIMBER_TOP_RIGHT, glm::vec2(float(i) / 8, 17.f / 32));

		sprite->setAnimationSpeed(CLIMBER_TOP_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(CLIMBER_TOP_LEFT, glm::vec2(float(i) / 8, 18.f / 32));

	/* Building */
		sprite->setAnimationSpeed(BUILDER_RIGHT, speed);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(BUILDER_RIGHT, glm::vec2(float(i) / 8, 19.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BUILDER_RIGHT, glm::vec2(float(i) / 8, 20.f / 32));

		sprite->setAnimationSpeed(BUILDER_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BUILDER_LEFT, glm::vec2(float(i) / 8, 21.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BUILDER_LEFT, glm::vec2(float(i) / 8, 22.f / 32));

	/* Builder Stoping */
		sprite->setAnimationSpeed(BUILDER_STOP_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BUILDER_STOP_RIGHT, glm::vec2(float(i) / 8, 23.f / 32));

		sprite->setAnimationSpeed(BUILDER_STOP_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BUILDER_STOP_LEFT, glm::vec2(float(i) / 8, 24.f / 32));

	/* Exiting */
		sprite->setAnimationSpeed(EXIT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(EXIT, glm::vec2(float(i) / 8, 25.f / 32));

	/* Dying Explosion */
		sprite->setAnimationSpeed(DIE_EXPLOSION, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 26.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 27.f / 32));

	/* Dying Fall */
		sprite->setAnimationSpeed(DIE_EXPLOSION, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 28.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 29.f / 32));


	
}





