#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
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
	oldState = nextState = WALKING_RIGHT_STATE;
	dig_time = basher_time = 0;

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

		/* FALLING */

		case FALLING_RIGHT_STATE:
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) {
				move(0, fall);
			}
			// no more falling
			else {
				//action triggered
				if (nextState == BLOCKER_TRIGGERED) {
					sprite->changeAnimation(BLOCKER);
					oldState = BLOCKER_STATE;
					state = BLOCKER_STATE;
					nextState = BLOCKER_STATE;
				}
				else if (nextState == DIGGER_TRIGGERED) {
					dig_time = 0;
					sprite->changeAnimation(DIGGER);
					oldState = WALKING_RIGHT_STATE;
					state = DIGGER_STATE;
					nextState = WALKING_RIGHT_STATE;
				}
				else if (nextState == BUILDER_TRIGGERED) {
					sprite->changeAnimation(BUILDER_RIGHT);
					oldState = WALKING_RIGHT_STATE;
					state = BUILDER_RIGHT_STATE;
					nextState = WALKING_RIGHT_STATE;
				}
				// no action triggered
				else {
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
				}
			}
			break;

		case FALLING_LEFT_STATE:
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) {		
				move(0, fall);
			}
			// no more falling
			else {				
				//action triggered
				if (nextState == BLOCKER_TRIGGERED) {
					sprite->changeAnimation(BLOCKER);
					oldState = BLOCKER_STATE;
					state = BLOCKER_STATE;
					nextState = BLOCKER_STATE;
				}
				else if (nextState == DIGGER_TRIGGERED) {
					dig_time = 0;
					sprite->changeAnimation(DIGGER);
					oldState = WALKING_LEFT_STATE;
					state = DIGGER_STATE;
					nextState = WALKING_LEFT_STATE;
				}
				else if (nextState == BUILDER_TRIGGERED) {
					sprite->changeAnimation(BUILDER_LEFT);
					oldState = WALKING_LEFT_STATE;
					state = BUILDER_LEFT_STATE;
					nextState = WALKING_LEFT_STATE;
				}
				// no action triggered
				else {		
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
				}
			}
			break;

		/* WALKING */
		case WALKING_RIGHT_STATE:
			move(1, -1);
			// wall
			if (collisionRight(4) < 4 && nextState == BASHER_TRIGGERED) {
				basher_time = 0;
				move(0, 1);
				sprite->changeAnimation(BASHER_RIGHT);
				oldState = WALKING_RIGHT_STATE;
				state = BASHER_RIGHT_STATE;
				nextState = WALKING_RIGHT_STATE;
			}
			else if (collision()) {
				// action triggered
				if (nextState == CLIMBER_TRIGGERED) {
					sprite->changeAnimation(CLIMBER_RIGHT);
					oldState = WALKING_RIGHT_STATE;
					state = CLIMBER_RIGHT_STATE;
					nextState = WALKING_RIGHT_STATE;
				}
				// no action triggered
				else {
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
				}
			}
			// no wall
			else {
				fall = collisionFloor(3);
				// no falling
				if (fall < 3) {
					move(0, fall);
					//action triggered
					if (nextState == BLOCKER_TRIGGERED) {
						sprite->changeAnimation(BLOCKER);
						oldState = BLOCKER_STATE;
						state = BLOCKER_STATE;
						nextState = BLOCKER_STATE;
						// TODO print on mask to stop lemmings ?
					}
					else if (nextState == DIGGER_TRIGGERED) {
						dig_time = 0;
						move(0, 1);
						sprite->changeAnimation(DIGGER);
						oldState = WALKING_RIGHT_STATE;
						state = DIGGER_STATE;
						nextState = WALKING_RIGHT_STATE;
					}
					else if (nextState == BUILDER_TRIGGERED) {
						sprite->changeAnimation(BUILDER_RIGHT);
						oldState = WALKING_RIGHT_STATE;
						state = BUILDER_RIGHT_STATE;
						nextState = WALKING_RIGHT_STATE;
					}
					// no action triggered -> continue walking
				}
				// falling
				else {
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
				}

			}
			break;

		case WALKING_LEFT_STATE:
			move(-1, -1);
			// wall
			if (collisionLeft(4) < 4 && nextState == BASHER_TRIGGERED) {
				basher_time = 0;
				sprite->changeAnimation(BASHER_LEFT);
				oldState = WALKING_LEFT_STATE;
				state = BASHER_LEFT_STATE;
				nextState = WALKING_LEFT_STATE;
			}
			else if(collision()) {
				// action triggered
				if (nextState == CLIMBER_TRIGGERED) {
					sprite->changeAnimation(CLIMBER_LEFT);
					oldState = WALKING_LEFT_STATE;
					state = CLIMBER_LEFT_STATE;
					nextState = WALKING_LEFT_STATE;
				}
				// no action triggered
				else {									
					move(1, 1);
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
				}
			}
			// no wall
			else {				
				fall = collisionFloor(3);
				// no falling
				if (fall < 3) {	
					move(0, fall);
					//action triggered
					if (nextState == BLOCKER_TRIGGERED) {
						sprite->changeAnimation(BLOCKER);
						oldState = BLOCKER_STATE;
						state = BLOCKER_STATE;
						nextState = BLOCKER_STATE;
						// TODO print on mask to stop lemmings ?
					}
					else if (nextState == DIGGER_TRIGGERED) {
						dig_time = 0;
						move(0, 1);
						sprite->changeAnimation(DIGGER);
						oldState = WALKING_LEFT_STATE;
						state = DIGGER_STATE;
						nextState = WALKING_LEFT_STATE;
					}
					else if (nextState == BUILDER_TRIGGERED) {
						sprite->changeAnimation(BUILDER_LEFT);
						oldState = WALKING_LEFT_STATE;
						state = BUILDER_LEFT_STATE;
						nextState = WALKING_LEFT_STATE;
					}
					// no action triggered -> continue walking
				}
				// falling
				else {				
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
			}
			break;

		/* Digging */
		case DIGGER_STATE:
			fall = collisionFloor(2);
			// falling
			if (fall > 1) {
				// left
				dig();
				if (oldState == WALKING_LEFT_STATE) {
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
				// right
				else {
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
				}
				move(0, 1);
				dig();
			}
			// no falling
			else {
				//action triggered -> stop, walk and trigger again
				if (nextState == BASHER_TRIGGERED ||
					nextState == CLIMBER_TRIGGERED ||
					nextState == BUILDER_TRIGGERED ||
					nextState == BLOCKER_TRIGGERED) {
					if (oldState == WALKING_LEFT_STATE) {
						sprite->changeAnimation(WALKING_LEFT);
						state = WALKING_LEFT_STATE;
					}
					else {
						sprite->changeAnimation(WALKING_RIGHT);
						state = WALKING_RIGHT_STATE;
					}
					
				}
				// no action triggered -> continue digging
				else {
					nextState = oldState;
					dig_time++;
					if (dig_time % 8 == 4) {
						dig();
						move(0, 1.f);
					}
				}
			}
			break;

		/* Blocking */
		case BLOCKER_STATE:
			// ? nothing to do ?
			break;
		
		/* Bashing */
		case BASHER_RIGHT_STATE:
			fall = collisionFloor(2);
			// falling
			if (fall > 0) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
			// no falling no wall
			else if (collisionRight(14) > 13) {
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
			// no falling wall -> continue bashing
			else {
				//action triggered -> stop, walk and trigger again
				if (nextState == CLIMBER_TRIGGERED ||
					nextState == BUILDER_TRIGGERED ||
					nextState == DIGGER_TRIGGERED ||
					nextState == BLOCKER_TRIGGERED) {
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
					
				}
				// no action triggered -> continue bashing
				else {
					nextState = oldState;
					basher_time++;
					if (basher_time % 16 > 10 && basher_time % 16 <= 15) {
						move(1.f, 0.f);
					}
					else if (basher_time % 16 > 0 && basher_time % 16 <= 6) {
						bashRight(basher_time % 16 + 2);
					}
				}
			}
			break;

		case BASHER_LEFT_STATE:
			fall = collisionFloor(2);
			// falling
			if (fall > 0) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
			// no falling no wall
			else if (collisionLeft(14) > 13) {
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
			// no falling no wall -> continue bashing
			else {
				//action triggered -> stop, walk and trigger again
				if (nextState == CLIMBER_TRIGGERED ||
					nextState == BUILDER_TRIGGERED ||
					nextState == DIGGER_TRIGGERED ||
					nextState == BLOCKER_TRIGGERED) {
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
					
				}
				// no action triggered -> continue bashing
				else {
					nextState = oldState;
					basher_time++;
					if (basher_time % 16 > 10 && basher_time % 16 <= 15) {
						move(-1.f, 0.f);
					}
					else if (basher_time % 16 > 0 && basher_time % 16 <= 6) {
						bashLeft(basher_time%16 + 1);
					}
				}
			}
			break;

		/* Climbing */
		case CLIMBER_RIGHT_STATE:
			break;

		case CLIMBER_LEFT_STATE:
			break;

		/* Building */
		case BUILDER_RIGHT_STATE:
			break;

		case BUILDER_LEFT_STATE:
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

int Lemming::collisionFloor(int maxFall) {
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position();
	
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

int Lemming::collisionRight(int maxWall) {
	bool bContact = false;
	int wall = 0;
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 13);

	while ((wall < maxWall) && !bContact)
	{
		if (mask->pixel(posBase.x + wall, posBase.y ) == 0)
			wall += 1;
		else
			bContact = true;
	}

	return wall;
}

int Lemming::collisionLeft(int maxWall) {
	bool bContact = false;
	int wall = 0;
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 13);

	while ((wall < maxWall) && !bContact)
	{
		if ((mask->pixel(posBase.x - wall, posBase.y) == 0) && (mask->pixel(posBase.x - wall, posBase.y - 1) == 0))
			wall += 1;
		else
			bContact = true;
	}

	return wall;
}

bool Lemming::collision() {
	glm::ivec2 posBase = sprite->position(); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::move(float x, float y) {
	sprite->position() += glm::vec2(x, y);
}

void Lemming::dig() {
	AudioEngine::instance().diggEffect();
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 16);
	for (int x = max(0, int(posBase.x - 3)); x <= min(mask->width(), int(posBase.x + 3)); x++)
		mask->setPixel(x, posBase.y - 2.f, 0);
}

void Lemming::bashLeft(int q) {
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 7);
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->width(), int(posBase.y + 8)); y++)
		mask->setPixel(posBase.x - q, y, 0);
}

void Lemming::bashRight(int q) {
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 7);
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->width(), int(posBase.y + 8)); y++)
		mask->setPixel(posBase.x + q, y, 0);
}


void Lemming::changeState(int x) {
	switch (x) {
		case 1:
			nextState = DIGGER_TRIGGERED;
			break;
		case 2:
			nextState = BLOCKER_TRIGGERED;
			break;
		case 3:
			nextState = BASHER_TRIGGERED;
			break;
		case 4: 
			nextState = CLIMBER_TRIGGERED;
			break;
		case 5:
			nextState = BUILDER_TRIGGERED;
			break;
		default:
			break;
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

		sprite->setAnimationSpeed(BASHER_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_LEFT, glm::vec2(float(i) / 8, 11.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_LEFT, glm::vec2(float(i) / 8, 12.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_LEFT, glm::vec2(float(i) / 8, 13.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BASHER_LEFT, glm::vec2(float(i) / 8, 14.f / 32));

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
		sprite->setAnimationSpeed(EXITING, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(EXITING, glm::vec2(float(i) / 8, 25.f / 32));

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





