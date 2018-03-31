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




Lemming::Lemming() {
	
}

Lemming::~Lemming() {
}

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *mask) {
	setMapMask(mask);
	state = FALLING_RIGHT_STATE;
	status = ALIVE_STATUS;
	oldState = nextState = WALKING_RIGHT_STATE;
	actionTime = 0;

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.03125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(23);

	setAnimations();

	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime) {
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	// control live, exit or bombs?

	switch(state)	{

		/* FALLING */
		case FALLING_RIGHT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				fall = collisionFloor(2);
				// more falling
				if (fall > 0) {
					// floater triggered
					if (actionTime > 8 && nextState == FLOATER_TRIGGERED) {
						actionTime = 0;
						sprite->changeAnimation(OPENING_UMBRELLA_RIGHT);
						oldState = WALKING_RIGHT_STATE;
						state = FLOATER_RIGHT_STATE;
						nextState = WALKING_RIGHT_STATE;
						move(0, 1);
					}
					// no floater
					else {
						move(0, fall);
					}
				}
				// no more falling
				else {
					// too high no floater -> die
					if (actionTime > 32) {
						actionTime = 0;
						sprite->changeAnimation(DIE_FALL);
						state = DYING_FALL_STATE;
					}
					// not too high and action triggered
					else if (nextState == BLOCKER_TRIGGERED) {
						sprite->changeAnimation(BLOCKER);
						oldState = BLOCKER_STATE;
						state = BLOCKER_STATE;
						nextState = BLOCKER_STATE;
					}
					else if (nextState == DIGGER_TRIGGERED) {
						actionTime = 0;
						move(0, 2);
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
			}
			break;

		case FALLING_LEFT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				fall = collisionFloor(2);
				// more falling
				if (fall > 0) {
					// floater triggered
					if (actionTime > 8 && nextState == FLOATER_TRIGGERED) {
						actionTime = 0;
						sprite->changeAnimation(OPENING_UMBRELLA_LEFT);
						oldState = WALKING_LEFT_STATE;
						state = FLOATER_LEFT_STATE;
						nextState = WALKING_LEFT_STATE;
						move(0, 1);
					}
					// no floater
					else {
						move(0, fall);
					}
				}
				// no more falling
				else {
					// too high no floater -> die
					if (actionTime > 32) {
						actionTime = 0;
						sprite->changeAnimation(DIE_FALL);
						state = DYING_FALL_STATE;
					}
					// not too high and action triggered
					else if (nextState == BLOCKER_TRIGGERED) {
						sprite->changeAnimation(BLOCKER);
						oldState = BLOCKER_STATE;
						state = BLOCKER_STATE;
						nextState = BLOCKER_STATE;
					}
					else if (nextState == DIGGER_TRIGGERED) {
						actionTime = 0;
						move(0, 2);
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
			}
			break;

		/* WALKING */
		case WALKING_RIGHT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				move(1, -1);
				// wall
				if (collisionRight(4) < 4 && nextState == BASHER_TRIGGERED) {
					actionTime = 0;
					move(0, 1);
					sprite->changeAnimation(BASHER_RIGHT);
					oldState = WALKING_RIGHT_STATE;
					state = BASHER_RIGHT_STATE;
					nextState = WALKING_RIGHT_STATE;
				}
				else if (collisionWalk()) {
					// action triggered
					if (nextState == CLIMBER_TRIGGERED) {
						actionTime = 0;
						sprite->changeAnimation(CLIMBER_RIGHT);
						oldState = WALKING_RIGHT_STATE;
						state = CLIMBER_RIGHT_STATE;
						nextState = CLIMBER_TRIGGERED;
					}
					// no action triggered
					else {
						move(-1, 1);
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
							actionTime = 0;
							move(0, 2);
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
						actionTime = 0;
						sprite->changeAnimation(FALLING_RIGHT);
						state = FALLING_RIGHT_STATE;
					}

				}
			}
			break;

		case WALKING_LEFT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				move(-1, -1);
				// wall
				if (collisionLeft(3) < 3 && nextState == BASHER_TRIGGERED) {
					actionTime = 0;
					move(0, 1);
					sprite->changeAnimation(BASHER_LEFT);
					oldState = WALKING_LEFT_STATE;
					state = BASHER_LEFT_STATE;
					nextState = WALKING_LEFT_STATE;
				}
				else if (collisionWalk()) {
					// action triggered
					if (nextState == CLIMBER_TRIGGERED) {
						actionTime = 0;
						sprite->changeAnimation(CLIMBER_LEFT);
						oldState = WALKING_LEFT_STATE;
						state = CLIMBER_LEFT_STATE;
						nextState = CLIMBER_TRIGGERED;
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
							actionTime = 0;
							move(0, 2);
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
						actionTime = 0;
						sprite->changeAnimation(FALLING_LEFT);
						state = FALLING_LEFT_STATE;
					}
				}
			}
			break;

		/* Digging */
		case DIGGER_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				fall = collisionFloor(4);
				// falling
				if (fall > 3) {
					// left
					dig();
					move(0, 1);

					if (oldState == WALKING_LEFT_STATE) {
						actionTime = 0;
						sprite->changeAnimation(FALLING_LEFT);
						state = FALLING_LEFT_STATE;
					}
					// right
					else {
						actionTime = 0;
						sprite->changeAnimation(FALLING_RIGHT);
						state = FALLING_RIGHT_STATE;
					}

				}
				// no falling
				else {
					//action triggered -> stop, walk and trigger again
					if (nextState == BASHER_TRIGGERED ||
						nextState == CLIMBER_TRIGGERED ||
						nextState == BUILDER_TRIGGERED ||
						nextState == BLOCKER_TRIGGERED) {
						dig();
						move(0, 1);
						dig();
						move(0, -1);
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
						actionTime++;
						if (actionTime % 8 == 4) {
							dig();
							move(0, 1);
						}
					}
				}
			}
			break;

		/* Blocking */
		case BLOCKER_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				// ? nothing to do ?
			}
			break;
		
		/* Bashing */
		case BASHER_RIGHT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				fall = collisionFloor(2);
				// falling
				if (fall > 0) {
					actionTime = 0;
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
						actionTime++;
						if (actionTime % 16 > 10 && actionTime % 16 <= 15) {
							move(1, 0);
						}
						else if (actionTime % 16 > 0 && actionTime % 16 <= 6) {
							bashRight(actionTime % 16 + 2);
						}
						if (actionTime % 16 == 1) AudioEngine::instance().diggEffect();
					}
				}
			}
			break;

		case BASHER_LEFT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				fall = collisionFloor(2);
				// falling
				if (fall > 0) {
					actionTime = 0;
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
				// no falling no wall
				else if (collisionLeft(13) > 12) {
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
						actionTime++;
						if (actionTime % 16 > 10 && actionTime % 16 <= 15) {
							move(-1, 0);
						}
						else if (actionTime % 16 > 0 && actionTime % 16 <= 6) {
							bashLeft(actionTime % 16 + 1);
						}
						if (actionTime % 16 == 1) AudioEngine::instance().diggEffect();
					}
				}
			}
			break;

		/* Climbing */
		case CLIMBER_RIGHT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				// no wall
				if (!collisionFullWall()) {
					actionTime = 0;
					sprite->changeAnimation(CLIMBER_TOP_RIGHT);
					state = CLIMBER_TOP_STATE;
				}
				else if (collisionHeadRight()) {
					actionTime = 0;
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
					move(-1, 0);
				}
				// wall
				else {
					if (actionTime % 8 > 4)
						move(0, -1);
				}
			}
			break;

		case CLIMBER_LEFT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				// no wall
				if (!collisionFullWall()) {
					actionTime = 0;
					sprite->changeAnimation(CLIMBER_TOP_LEFT);
					state = CLIMBER_TOP_STATE;
				}
				// head collision
				else if (collisionHeadLeft()) {
					actionTime = 0;
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
					move(1, 0);
				}
				// wall
				else {
					if (actionTime % 8 > 4)
						move(0, -1);
				}
			}
			break;

		case CLIMBER_TOP_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				if (actionTime == 8) {
					if (oldState == WALKING_RIGHT_STATE) {
						sprite->changeAnimation(WALKING_RIGHT);
						state = WALKING_RIGHT_STATE;
					}
					else {
						sprite->changeAnimation(WALKING_LEFT);
						state = WALKING_LEFT_STATE;
					}
					move(0, -7);
				}
			}
			break;


		/* Building */
		case BUILDER_RIGHT_STATE:
			break;

		case BUILDER_LEFT_STATE:
			break;


		/* Floating */
		case FLOATER_RIGHT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				fall = collisionFloor(2);
				if (actionTime == 4) sprite->changeAnimation(FLOATER_RIGHT);
				// more falling
				if (fall > 0) {
					move(0, fall);
				}
				// no more falling
				else {
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
				}
			}
			break;

		case FLOATER_LEFT_STATE:
			// die triggered
			if (nextState == DYING_EXPLOSION_TRIGGERED) {
				goDieExplosion();
			}
			else {
				actionTime++;
				fall = collisionFloor(2);
				if (actionTime == 4) sprite->changeAnimation(FLOATER_LEFT);
				// more falling
				if (fall > 0) {
					move(0, fall);
				}
				// no more falling
				else {
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
				}
			}
			break;

		case DYING_FALL_STATE:
			actionTime++;
			if (actionTime > 8) status = DEAD_STATUS;
			break;

		case DYING_EXPLOSION_STATE:
			actionTime++;
			if (actionTime > 16) status = DEAD_STATUS;
			break;

		default:
			break;
	}
}

void Lemming::goDieExplosion() {
	actionTime = 0;
	sprite->changeAnimation(DIE_EXPLOSION);
	state = DYING_EXPLOSION_STATE;
}

void Lemming::render() {
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask) {
	mask = mapMask;
}

// TODO a�adir todas las animaciones
void Lemming::doubleSpeed()
{
	sprite->setAnimationSpeed(WALKING_RIGHT, speed*2);
	sprite->setAnimationSpeed(WALKING_LEFT, speed * 2);
	sprite->setAnimationSpeed(FALLING_RIGHT, speed * 2);
	sprite->setAnimationSpeed(FALLING_LEFT, speed * 2);
	sprite->setAnimationSpeed(DIGGER, speed * 2);
	sprite->setAnimationSpeed(BLOCKER, speed * 2);
	sprite->setAnimationSpeed(BASHER_RIGHT, speed * 2);
	sprite->setAnimationSpeed(BASHER_LEFT, speed * 2);
	sprite->setAnimationSpeed(CLIMBER_RIGHT, speed * 2);
	sprite->setAnimationSpeed(CLIMBER_LEFT, speed * 2);
	sprite->setAnimationSpeed(CLIMBER_TOP_RIGHT, speed * 2);
	sprite->setAnimationSpeed(CLIMBER_TOP_LEFT, speed * 2);
	sprite->setAnimationSpeed(BUILDER_RIGHT, speed * 2);
	sprite->setAnimationSpeed(BUILDER_LEFT, speed * 2);
	sprite->setAnimationSpeed(BUILDER_STOP_RIGHT, speed * 2);
	sprite->setAnimationSpeed(EXITING, speed * 2);
	sprite->setAnimationSpeed(FLOATER_RIGHT, speed*2);
	sprite->setAnimationSpeed(FLOATER_LEFT, speed*2);
	sprite->setAnimationSpeed(DYING_FALL_STATE, speed * 2);
}

void Lemming::resetSpeed()
{
	sprite->setAnimationSpeed(WALKING_RIGHT, speed);
	sprite->setAnimationSpeed(WALKING_LEFT, speed);
	sprite->setAnimationSpeed(FALLING_RIGHT, speed);
	sprite->setAnimationSpeed(FALLING_LEFT, speed);
	sprite->setAnimationSpeed(DIGGER, speed);
	sprite->setAnimationSpeed(BLOCKER, speed);
	sprite->setAnimationSpeed(BASHER_RIGHT, speed);
	sprite->setAnimationSpeed(BASHER_LEFT, speed);
	sprite->setAnimationSpeed(CLIMBER_RIGHT, speed);
	sprite->setAnimationSpeed(CLIMBER_LEFT, speed);
	sprite->setAnimationSpeed(CLIMBER_TOP_RIGHT, speed);
	sprite->setAnimationSpeed(CLIMBER_TOP_LEFT, speed);
	sprite->setAnimationSpeed(BUILDER_RIGHT, speed);
	sprite->setAnimationSpeed(BUILDER_LEFT, speed);
	sprite->setAnimationSpeed(BUILDER_STOP_RIGHT, speed);
	sprite->setAnimationSpeed(EXITING, speed);
	sprite->setAnimationSpeed(FLOATER_RIGHT, speed);
	sprite->setAnimationSpeed(FLOATER_LEFT, speed);
	sprite->setAnimationSpeed(DYING_FALL_STATE, speed);
}

void Lemming::pause()
{
	sprite->setAnimationSpeed(WALKING_RIGHT, 0);
	sprite->setAnimationSpeed(WALKING_LEFT, 0);
	sprite->setAnimationSpeed(FALLING_RIGHT, 0);
	sprite->setAnimationSpeed(FALLING_LEFT, 0);
	sprite->setAnimationSpeed(DIGGER, 0);
	sprite->setAnimationSpeed(BLOCKER, 0);
	sprite->setAnimationSpeed(BASHER_RIGHT, 0);
	sprite->setAnimationSpeed(BASHER_LEFT, 0);
	sprite->setAnimationSpeed(CLIMBER_RIGHT, 0);
	sprite->setAnimationSpeed(CLIMBER_LEFT, 0);
	sprite->setAnimationSpeed(CLIMBER_TOP_RIGHT, 0);
	sprite->setAnimationSpeed(CLIMBER_TOP_LEFT, 0);
	sprite->setAnimationSpeed(BUILDER_RIGHT, 0);
	sprite->setAnimationSpeed(BUILDER_LEFT, 0);
	sprite->setAnimationSpeed(BUILDER_STOP_RIGHT, 0);
	sprite->setAnimationSpeed(EXITING, 0);
	sprite->setAnimationSpeed(FLOATER_RIGHT, 0);
	sprite->setAnimationSpeed(FLOATER_LEFT, 0);
	sprite->setAnimationSpeed(DYING_FALL_STATE, 0);
}

glm::vec2 Lemming::getPosition() {
	return sprite->position();
}

int Lemming::getStatus()
{
	return status;
}

int Lemming::collisionFloor(int maxFall) {
	bool bContact = false;
	int fall = -1;
	glm::ivec2 posBase = sprite->position();
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact) {
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
		if ((mask->pixel(posBase.x - wall, posBase.y) == 0))
			wall += 1;
		else
			bContact = true;
	}

	return wall;
}

bool Lemming::collisionWalk() {
	glm::ivec2 posBase = sprite->position(); // Add the map displacement
	
	posBase += glm::ivec2(7, 14);
	for (int y = posBase.y; y < posBase.y + 1; y++) {
		if ((mask->pixel(posBase.x, y) == 0) && (mask->pixel(posBase.x + 1, y) == 0))
			return false;
	}

	return true;
}

bool Lemming::collisionFullWall() {
	glm::ivec2 posBase = sprite->position(); // Add the map displacement

	posBase += glm::ivec2(7, 7);
	for (int y = posBase.y; y < posBase.y + 7; y++) {
		if ((mask->pixel(posBase.x, y) == 0) && (mask->pixel(posBase.x + 1, y) == 0))
			return false;
	}

	return true;
}

bool Lemming::collisionHeadRight() {
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 4);
	if ((mask->pixel(posBase.x, posBase.y) == 0))
		return false;
	return true;
}

bool Lemming::collisionHeadLeft() {
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 4);
	if ((mask->pixel(posBase.x + 1, posBase.y) == 0))
		return false;
	return true;
}

void Lemming::move(int x, int y) {
	sprite->position() += glm::vec2(x, y);
}

void Lemming::dig() {
	AudioEngine::instance().diggEffect();
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 16);
	for (int x = max(0, int(posBase.x - 3)); x <= min(mask->width(), int(posBase.x + 3)); x++)
		mask->setPixel(x, int(posBase.y) - 2, 0);
}

void Lemming::bashLeft(int q) {
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 7);
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->width(), int(posBase.y + 8)); y++)
		mask->setPixel(int(posBase.x) - q, y, 0);
}

void Lemming::bashRight(int q) {
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 7);
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->width(), int(posBase.y + 8)); y++)
		mask->setPixel(int(posBase.x) + q, y, 0);
}

void Lemming::changeState(int actionTriggered) {
	switch (actionTriggered) {
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
		case 6: 
			nextState = FLOATER_TRIGGERED;
			break;
		case 7:
			nextState = BOMBER_TRIGGERED;
			break;
		case 8:
			nextState = DYING_EXPLOSION_TRIGGERED;
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
		sprite->setAnimationSpeed(DIE_EXPLOSION, 10);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 26.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_EXPLOSION, glm::vec2(float(i) / 8, 27.f / 32));

	/* Dying Fall */
		sprite->setAnimationSpeed(DIE_FALL, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_FALL, glm::vec2(float(i) / 8, 28.f / 32));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIE_FALL, glm::vec2(float(i) / 8, 29.f / 32));

	/* Opening Umbrella */
		sprite->setAnimationSpeed(OPENING_UMBRELLA_RIGHT, speed);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(OPENING_UMBRELLA_RIGHT, glm::vec2(float(i) / 8, 30.f / 32));

		sprite->setAnimationSpeed(OPENING_UMBRELLA_LEFT, speed);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(OPENING_UMBRELLA_LEFT, glm::vec2(float(i) / 8, 31.f / 32));

	/* Floating */
		sprite->setAnimationSpeed(FLOATER_RIGHT, speed/2);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(FLOATER_RIGHT, glm::vec2(float(i) / 8, 30.f / 32));
		for (int i = 7; i>4; i--)
			sprite->addKeyframe(FLOATER_RIGHT, glm::vec2(float(i) / 8, 30.f / 32));

		sprite->setAnimationSpeed(FLOATER_LEFT, speed/2);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(FLOATER_LEFT, glm::vec2(float(i) / 8, 31.f / 32));
		for (int i = 7; i>4; i--)
			sprite->addKeyframe(FLOATER_LEFT, glm::vec2(float(i) / 8, 31.f / 32));


	
}





