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
	if (sprite != NULL)
		delete sprite;
	if (map != NULL)
		delete map;
	if (mask != NULL)
		delete mask;
}

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *map, VariableTexture *mask) {
	
	setMapMask(map, mask);
	state = FALLING_RIGHT_STATE;
	status = ALIVE_STATUS;
	oldState = nextState = WALKING_RIGHT_STATE;
	resetActionTime();

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.03125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(25);

	setAnimations();
	

	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime) {
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	// control live, exit or bombs?

	actionTime++;
	

	// die triggered
	if (nextState == DYING_EXPLOSION_TRIGGERED)
		goDieExplosion();	

	switch(state)	{

		/* FALLING */
		case FALLING_RIGHT_STATE:
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) {
				// floater triggered
				if (actionTime > 8 && nextState == FLOATER_TRIGGERED) 
					goFloaterRight();

				// no floater
				else 
					move(0, fall);
			}
			// no more falling
			else {
				// too high no floater -> die
				if (actionTime > 32) 
					goDieFall();
					
				// not too high and action triggered
				else if (nextState == BLOCKER_TRIGGERED) 
					goBlocker();
					
				else if (nextState == DIGGER_TRIGGERED) 
					goDigger(WALKING_RIGHT_STATE);
					
				else if (nextState == BUILDER_TRIGGERED) 
					goBuilderRight();
					
				// no action triggered
				else 
					goWalkRight();
			}
			break;

		case FALLING_LEFT_STATE:
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) {
				// floater triggered
				if (actionTime > 8 && nextState == FLOATER_TRIGGERED) 
					goFloaterLeft();
					
				// no floater
				else 
					move(0, fall);	
			}
			// no more falling
			else {
				// too high no floater -> die
				if (actionTime > 32) 
					goDieFall();
					
				// not too high and action triggered
				else if (nextState == BLOCKER_TRIGGERED) 
					goBlocker();
					
				else if (nextState == DIGGER_TRIGGERED) 
					goDigger(WALKING_LEFT_STATE);
					
				else if (nextState == BUILDER_TRIGGERED) 
					goBuilderLeft();
					
				// no action triggered
				else 
					goWalkLeft();
			}
			break;

		/* WALKING */
		case WALKING_RIGHT_STATE:
			move(1, -1);
			// wall
			if (collisionRight(4) < 4 && nextState == BASHER_TRIGGERED) 
				goBasherRight();
				
			else if (collisionWalk()) {
				// action triggered
				if (collisionFullWall() && nextState == CLIMBER_TRIGGERED) 
					goClimberRight();
					
				// no action triggered
				else {
					move(-1, 1);
					goWalkLeft();
				}
			}
			// no wall
			else {
				fall = collisionFloor(4);
				// no falling
				if (fall < 4) {
					move(0, fall);
					//action triggered
					if (nextState == BLOCKER_TRIGGERED)
						goBlocker();

					else if (nextState == DIGGER_TRIGGERED)
						goDigger(WALKING_RIGHT_STATE);

					else if (nextState == BUILDER_TRIGGERED)
						goBuilderRight();

					else if (nextState == BOMBER_TRIGGERED)
						goBomberRight();
						
					// no action triggered -> continue walking
				}
				// falling
				else {
					move(0, 1);
					goFallRight();
				}
					
			}
			break;

		case WALKING_LEFT_STATE:
			move(-1, -1);
			// wall
			if (collisionLeft(3) < 3 && nextState == BASHER_TRIGGERED) 
				goBasherLeft();
				
			else if (collisionWalk()) {
				// action triggered
				if (collisionFullWall() && nextState == CLIMBER_TRIGGERED)
					goClimberLeft();
					
				// no action triggered
				else {
					move(1, 1);
					goWalkRight();
				}
			}
			// no wall
			else {
				fall = collisionFloor(4);
				// no falling
				if (fall < 4) {
					move(0, fall);
					//action triggered
					if (nextState == BLOCKER_TRIGGERED) 
						goBlocker();
						
					else if (nextState == DIGGER_TRIGGERED) 
						goDigger(WALKING_LEFT_STATE);
						
					else if (nextState == BUILDER_TRIGGERED) 
						goBuilderLeft();
					
					else if (nextState == BOMBER_TRIGGERED)
						goBomberLeft();

					// no action triggered -> continue walking
				}
				// falling
				else {
					move(0, 1);
					goFallLeft();
				}
			}
			break;

		/* Digging */
		case DIGGER_STATE:
			fall = collisionFloor(4);
			// falling
			if (fall > 3) {
				dig();
				move(0, 1);

				// left
				if (oldState == WALKING_LEFT_STATE) goFallLeft();
				// right
				else goFallRight();
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
					if (oldState == WALKING_LEFT_STATE) goWalkLeft();
					else goWalkRight();
				}
				// no action triggered -> continue digging
				else {
					nextState = oldState;
					if (actionTime % 8 == 4) {
						dig();
						move(0, 1);
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
			if (fall > 0) goFallRight();
			// no falling no wall
			else if (collisionRight(14) > 13) goWalkRight();
			// no falling wall -> continue bashing
			else {
				//action triggered -> stop, walk and trigger again
				if (nextState == CLIMBER_TRIGGERED ||
					nextState == BUILDER_TRIGGERED ||
					nextState == DIGGER_TRIGGERED ||
					nextState == BLOCKER_TRIGGERED) 
					goWalkRight();

				// no action triggered -> continue bashing
				else {
					nextState = oldState;
					if (actionTime % 16 > 10 && actionTime % 16 <= 15)
						move(1, 0);
					else if (actionTime % 16 > 0 && actionTime % 16 <= 6) 
						bashRight(actionTime % 16 + 2);
	
					if (actionTime % 16 == 1) AudioEngine::instance().diggEffect();
				}
			}
			break;

		case BASHER_LEFT_STATE:
			fall = collisionFloor(2);
			// falling
			if (fall > 0) goFallLeft();
			// no falling no wall
			else if (collisionLeft(13) > 12) goWalkLeft();
			// no falling wall -> continue bashing
			else {
				//action triggered -> stop, walk and trigger again
				if (nextState == CLIMBER_TRIGGERED ||
					nextState == BUILDER_TRIGGERED ||
					nextState == DIGGER_TRIGGERED ||
					nextState == BLOCKER_TRIGGERED ||
					nextState == BOMBER_TRIGGERED) 
					goWalkLeft();
				// no action triggered -> continue bashing
				else {
					nextState = oldState;
					if (actionTime % 16 > 10 && actionTime % 16 <= 15) 
						move(-1, 0);
					else if (actionTime % 16 > 0 && actionTime % 16 <= 6) 
						bashLeft(actionTime % 16 + 1);
					
					if (actionTime % 16 == 1) AudioEngine::instance().diggEffect();
				}
			}
			break;


		/* Climbing */
		case CLIMBER_RIGHT_STATE:
			// no wall -> top of wall
			if (!collisionFullWall()) {
				resetActionTime();
				sprite->changeAnimation(CLIMBER_TOP_RIGHT);
				state = CLIMBER_TOP_STATE;
			}
			// head collision -> fall not facing fall
			else if (collisionHeadRight()) {
				goFallLeft();
				move(-1, 0);
			}
			// wall
			else if (actionTime % 8 > 4) move(0, -1);
			break;

		case CLIMBER_LEFT_STATE:
			// no wall -> top of wall
			if (!collisionFullWall()) {
				resetActionTime();
				sprite->changeAnimation(CLIMBER_TOP_LEFT);
				state = CLIMBER_TOP_STATE;
			}
			// head collision -> fall not facing fall
			else if (collisionHeadLeft()) {
				goFallRight();
				move(1, 0);
			}
			// wall
			else if (actionTime % 8 > 4) move(0, -1);
			
			break;

		case CLIMBER_TOP_STATE:
			if (actionTime > 7) {
				if (oldState == WALKING_RIGHT_STATE) goWalkRight();
				else goWalkLeft();
				move(0, -7);
			}
			break;


		/* Building */ // TODO triggers should stop builder
		case BUILDER_RIGHT_STATE:
			// all tiles done
			if (counter == 12) goStopBuilderRight();
			//action triggered -> stop, walk and trigger again
			else if (nextState == CLIMBER_TRIGGERED ||
				nextState == DIGGER_TRIGGERED ||
				nextState == BLOCKER_TRIGGERED ||
				nextState == BOMBER_TRIGGERED ||
				nextState == BASHER_TRIGGERED)
				goWalkRight();
			// wall
			else if (collisionRight(15) < 8) goStopBuilderRight();
			// head collision
			else if (collisionHeadRight()) goStopBuilderRight();
			// still tiles to do and no collisions
			else {
				if (actionTime % 16 == 8) build(glm::vec2(9, 15));
				if (actionTime % 16 == 0) move(2, -1);
			}
			break;

		case BUILDER_LEFT_STATE:
			// all tiles done
			if (counter == 12) goStopBuilderLeft();
			//action triggered -> stop, walk and trigger again
			else if (nextState == CLIMBER_TRIGGERED ||
				nextState == BASHER_TRIGGERED ||
				nextState == DIGGER_TRIGGERED ||
				nextState == BLOCKER_TRIGGERED ||
				nextState == BOMBER_TRIGGERED)
				goWalkLeft();
			// wall
			else if (collisionLeft(15) < 6) goStopBuilderLeft();
			// head collision
			else if (collisionHeadLeft()) goStopBuilderLeft();
			// still tiles to do and no collisions
			else {
				if (actionTime % 16 == 8) build(glm::vec2(2, 15));
				if (actionTime % 16 == 0) move(-2, -1);
			}
			break;

		case BUILDER_STOP_STATE:
			if (actionTime > 8) {
				// left
				if (oldState == WALKING_LEFT_STATE) goWalkLeft();
				// right
				else goWalkRight();
			}
			break;


		/* Floating */
		case FLOATER_RIGHT_STATE:
			if (actionTime == 4) sprite->changeAnimation(FLOATER_RIGHT);
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) move(0, fall);
			// no more falling
			else goWalkRight();
			break;

		case FLOATER_LEFT_STATE:
			if (actionTime == 4) sprite->changeAnimation(FLOATER_LEFT);
			fall = collisionFloor(2);
			// more falling
			if (fall > 0) move(0, fall);
			// no more falling
			else goWalkLeft();
			break;

		
		/* Bomber */
		case BOMBER_RIGHT_STATE:
			if (actionTime > 7) goWalkLeft();
			break;

		case BOMBER_LEFT_STATE:
			if (actionTime > 7) goWalkRight();


		/* Dying */
		case DYING_FALL_STATE:
			if (actionTime > 14) goKill();
			break;

		case DYING_EXPLOSION_STATE:
			if (actionTime > 14) {
				explode();
				goKill();
			}
			break;

		/* Exiting */
		case EXITING_STATE:
			if (actionTime > 6) status = EXITED_STATUS;
			break;

		default:
			break;
	}

	
}

void Lemming::goDieExplosion() {
	resetActionTime();
	sprite->changeAnimation(DIE_EXPLOSION);
	state = DYING_EXPLOSION_STATE;
	nextState = DYING_EXPLOSION_STATE;
}

void Lemming::goDieFall() {
	AudioEngine::instance().splatEffect();
	resetActionTime();
	sprite->changeAnimation(DIE_FALL);
	state = DYING_FALL_STATE;
}

void Lemming::goBlocker() {
	block();
	sprite->changeAnimation(BLOCKER);
	oldState = nextState = state = BLOCKER_STATE;
}

void Lemming::goDigger(LemmingState oldNewState) {
	resetActionTime();
	move(0, 2);
	sprite->changeAnimation(DIGGER);
	oldState = nextState = oldNewState;
	state = DIGGER_STATE;
}

void Lemming::goFloaterRight() {
	AudioEngine::instance().umbrellaEffect();
	resetActionTime();
	move(0, 1);
	sprite->changeAnimation(OPENING_UMBRELLA_RIGHT);
	oldState = WALKING_RIGHT_STATE;
	state = FLOATER_RIGHT_STATE;
	nextState = FLOATER_TRIGGERED;
	
}

void Lemming::goFloaterLeft() {
	AudioEngine::instance().umbrellaEffect();
	resetActionTime();
	move(0, 1);
	sprite->changeAnimation(OPENING_UMBRELLA_LEFT);
	oldState = WALKING_LEFT_STATE;
	state = FLOATER_LEFT_STATE;
	nextState = FLOATER_TRIGGERED;

}

void Lemming::goBasherRight() {
	resetActionTime();
	move(0, 1);
	sprite->changeAnimation(BASHER_RIGHT);
	oldState = nextState = WALKING_RIGHT_STATE;
	state = BASHER_RIGHT_STATE;
}

void Lemming::goBasherLeft() {
	resetActionTime();
	move(0, 1);
	sprite->changeAnimation(BASHER_LEFT);
	oldState = nextState = WALKING_LEFT_STATE;
	state = BASHER_LEFT_STATE;
}

void Lemming::goBuilderRight() {
	resetActionTime();
	counter = 0;
	sprite->changeAnimation(BUILDER_RIGHT);
	oldState = nextState = WALKING_RIGHT_STATE;
	state = BUILDER_RIGHT_STATE;
}

void Lemming::goBuilderLeft() {
	resetActionTime();
	counter = 0;
	sprite->changeAnimation(BUILDER_LEFT);
	oldState = nextState = WALKING_LEFT_STATE;
	state = BUILDER_LEFT_STATE;
}

void Lemming::goStopBuilderRight() {
	resetActionTime();
	sprite->changeAnimation(BUILDER_STOP_RIGHT);
	state = BUILDER_STOP_STATE;
}

void Lemming::goStopBuilderLeft() {
	resetActionTime();
	sprite->changeAnimation(BUILDER_STOP_LEFT);
	state = BUILDER_STOP_STATE;
}

void Lemming::goClimberRight() {
	resetActionTime();
	sprite->changeAnimation(CLIMBER_RIGHT);
	oldState = WALKING_RIGHT_STATE;
	state = CLIMBER_RIGHT_STATE;
	nextState = CLIMBER_TRIGGERED;
}

void Lemming::goClimberLeft() {
	resetActionTime();
	sprite->changeAnimation(CLIMBER_LEFT);
	oldState = WALKING_LEFT_STATE;
	state = CLIMBER_LEFT_STATE;
	nextState = CLIMBER_TRIGGERED;
}

void Lemming::goBomberRight() {
	resetActionTime();
	sprite->changeAnimation(BOMBER_RIGHT);
	oldState = WALKING_RIGHT_STATE;
	nextState = WALKING_LEFT_STATE;
	state = BOMBER_RIGHT_STATE;
}

void Lemming::goBomberLeft() {
	resetActionTime();
	sprite->changeAnimation(BOMBER_LEFT);
	oldState = WALKING_LEFT_STATE;
	nextState = WALKING_RIGHT_STATE;
	state = BOMBER_LEFT_STATE;
}


void Lemming::goWalkLeft() {
	sprite->changeAnimation(WALKING_LEFT);
	state = WALKING_LEFT_STATE;
}

void Lemming::goWalkRight() {
	sprite->changeAnimation(WALKING_RIGHT);
	state = WALKING_RIGHT_STATE;
}

void Lemming::goFallLeft() {
	resetActionTime();
	sprite->changeAnimation(FALLING_LEFT);
	state = FALLING_LEFT_STATE;
}

void Lemming::goFallRight() {
	resetActionTime();
	sprite->changeAnimation(FALLING_RIGHT);
	state = FALLING_RIGHT_STATE;
}

void Lemming::goExplode() {
	//explode();
	//status = EXPLODED_STATUS;
}

void Lemming::goKill() {
	status = DEAD_STATUS;
}

void Lemming::goExit() {
	if (state != EXITING_STATE) {
		AudioEngine::instance().enterDoorEffect();
		resetActionTime();
		sprite->changeAnimation(EXITING);
		state = EXITING_STATE;
	}
}

void Lemming::setPosition(glm::vec2 newPosition) {
	sprite->position() = newPosition;
}



void Lemming::resetActionTime() {
	actionTime = 0;
}

void Lemming::render() {
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMap, VariableTexture *mapMask) {
	map = mapMap;
	mask = mapMask;
}

void Lemming::doubleSpeed() {
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
	sprite->setAnimationSpeed(BUILDER_STOP_LEFT, speed * 2);
	sprite->setAnimationSpeed(EXITING, speed * 2);
	sprite->setAnimationSpeed(DIE_FALL, speed * 2);
	sprite->setAnimationSpeed(DIE_EXPLOSION, 20);
	sprite->setAnimationSpeed(FLOATER_RIGHT, speed);
	sprite->setAnimationSpeed(FLOATER_LEFT, speed);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_LEFT, speed * 2);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_RIGHT, speed * 2);
	sprite->setAnimationSpeed(BOMBER_RIGHT, speed * 2);
	sprite->setAnimationSpeed(BOMBER_LEFT, speed * 2);
}

void Lemming::resetSpeed() {
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
	sprite->setAnimationSpeed(BUILDER_STOP_LEFT, speed);
	sprite->setAnimationSpeed(EXITING, speed);
	sprite->setAnimationSpeed(DIE_EXPLOSION, 10);
	sprite->setAnimationSpeed(FLOATER_RIGHT, speed/2);
	sprite->setAnimationSpeed(FLOATER_LEFT, speed/2);
	sprite->setAnimationSpeed(DIE_FALL, speed);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_LEFT, speed);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_RIGHT, speed);
	sprite->setAnimationSpeed(BOMBER_RIGHT, speed);
	sprite->setAnimationSpeed(BOMBER_LEFT, speed);
}

void Lemming::pause() {
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
	sprite->setAnimationSpeed(BUILDER_STOP_LEFT, 0);
	sprite->setAnimationSpeed(EXITING, 0);
	sprite->setAnimationSpeed(DIE_EXPLOSION, 0);
	sprite->setAnimationSpeed(FLOATER_RIGHT, 0);
	sprite->setAnimationSpeed(FLOATER_LEFT, 0);
	sprite->setAnimationSpeed(DIE_FALL, 0);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_LEFT, 0);
	sprite->setAnimationSpeed(OPENING_UMBRELLA_RIGHT, 0);
	sprite->setAnimationSpeed(BOMBER_RIGHT, 0);
	sprite->setAnimationSpeed(BOMBER_LEFT, 0);
}

glm::vec2 Lemming::getPosition() {
	return sprite->position();
}

Lemming::LemmingStatus Lemming::getStatus() {
	return status;
}

Lemming::LemmingState Lemming::getState() {
	return state;
}

Lemming::LemmingState Lemming::getNextState() {
	return nextState;
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
		if (mask->pixel(posBase.x + wall, posBase.y ) < 255)
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
		if ((mask->pixel(posBase.x - wall, posBase.y) < 255))
			wall += 1;
		else
			bContact = true;
	}

	return wall;
}

bool Lemming::collisionWalk() {
	glm::ivec2 posBase = sprite->position() + glm::vec2(7,7); 
	
	int x = posBase.x;
	int y = posBase.y;

	for (y = posBase.y; y < posBase.y + 7; y++) {
		if ((mask->pixel(posBase.x, y) > 253) || (mask->pixel(posBase.x + 1, y) > 253))
			return true;
	}

	return false;
}

bool Lemming::collisionFullWall() {
	glm::ivec2 posBase = sprite->position(); // Add the map displacement

	posBase += glm::ivec2(7, 7);
	for (int y = posBase.y; y < posBase.y + 7; y++) {
		if ((mask->pixel(posBase.x, y) < 255) && (mask->pixel(posBase.x + 1, y) < 255))
			return false;
	}
	return true;
}

bool Lemming::collisionHeadRight() {
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 4);
	if ((mask->pixel(posBase.x, posBase.y) < 254))
		return false;
	return true;
}

bool Lemming::collisionHeadLeft() {
	glm::ivec2 posBase = sprite->position();
	posBase += glm::ivec2(7, 4);
	if ((mask->pixel(posBase.x + 1, posBase.y) < 254))
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
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->height(), int(posBase.y + 8)); y++)
		mask->setPixel(int(posBase.x) - q, y, 0);
}

void Lemming::bashRight(int q) {
	glm::vec2 posBase = sprite->position() + glm::vec2(7, 7);
	for (int y = max(0, int(posBase.y-1)); y <= min(mask->height(), int(posBase.y + 8)); y++)
		mask->setPixel(int(posBase.x) + q, y, 0);
}

void Lemming::build(glm::vec2 offset) {
	counter++;
	glm::vec2 posBase = sprite->position() + offset;
	for (int x = max(0, int(posBase.x)); x <= min(map->width(), int(posBase.x + 6)); x++) {
		mask->setPixel(x, int(posBase.y), 253);
		map->setPixel(x, int(posBase.y), glm::ivec4(120,77,0,255));
	}	
}

void Lemming::block() {
	glm::vec2 posBase = sprite->position() + glm::vec2(3, 6);
	for (int x = max(0, int(posBase.x)); x < min(map->width(), int(posBase.x + 10)); x++) {
		for (int y = max(0, int(posBase.y)); y < min(mask->height(), int(posBase.y + 9)); y++) {
			mask->setPixel(x, y, 254);
			map->setPixel(x, y, glm::ivec4(0, 0, 0, 0));
		}
	}
}

void Lemming::explode() {
	AudioEngine::instance().explosionEffect();
	glm::ivec2 position = sprite->position();
	deleteMask(position.x + 6, position.x + 9, position.y + 0);
	deleteMask(position.x + 3, position.x + 12, position.y + 1);
	deleteMask(position.x + 1, position.x + 14, position.y + 2);
	deleteMask(position.x + 0, position.x + 15, position.y + 3);
	deleteMask(position.x + -1, position.x + 16, position.y + 4);
	deleteMask(position.x + -2, position.x + 17, position.y + 5);
	deleteMask(position.x + -2, position.x + 17, position.y + 6);
	deleteMask(position.x + -3, position.x + 18, position.y + 7);
	deleteMask(position.x + -3, position.x + 18, position.y + 8);
	deleteMask(position.x + -3, position.x + 18, position.y + 9);
	deleteMask(position.x + -4, position.x + 19, position.y + 10);
	deleteMask(position.x + -4, position.x + 19, position.y + 11);
	deleteMask(position.x + -4, position.x + 19, position.y + 12);
	deleteMask(position.x + -4, position.x + 19, position.y + 13);
	deleteMask(position.x + -3, position.x + 18, position.y + 14);
	deleteMask(position.x + -3, position.x + 18, position.y + 15);
	deleteMask(position.x + -3, position.x + 18, position.y + 16);
	deleteMask(position.x + -2, position.x + 17, position.y + 17);
	deleteMask(position.x + -2, position.x + 17, position.y + 18);
	deleteMask(position.x + -1, position.x + 16, position.y + 19);
	deleteMask(position.x + 0, position.x + 15, position.y + 20);
	deleteMask(position.x + 1, position.x + 14, position.y + 21);
	deleteMask(position.x + 3, position.x + 12, position.y + 22);
	deleteMask(position.x + 6, position.x + 9, position.y + 23);
}

void Lemming::deleteMask(int xStart, int xEnd, int y) {
	if (y > 0 && y < mask->height())
		for (int x = max(0, xStart); x <= min(mask->width(),xEnd); x++)
			mask->setPixel(x, y, 0);
}

void Lemming::changeState(int actionTriggered) {
	switch (actionTriggered) {
	case 0:
		nextState = BASHER_TRIGGERED;
		break;
	case 1:
		nextState = BLOCKER_TRIGGERED;
		break;
	case 2:
		nextState = CLIMBER_TRIGGERED;
		break;
	case 3:
		nextState = DIGGER_TRIGGERED;
		break;
	case 4:
		nextState = FLOATER_TRIGGERED;
		break;
	case 5:
		nextState = BOMBER_TRIGGERED;
		break;
	case 6:
		nextState = BUILDER_TRIGGERED;
		break;
	case 7:
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

	/* Bomber */
		sprite->setAnimationSpeed(BOMBER_RIGHT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BOMBER_RIGHT, glm::vec2(float(i) / 8, 23.f / 32));

		sprite->setAnimationSpeed(BOMBER_LEFT, speed);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BOMBER_LEFT, glm::vec2(float(i) / 8, 24.f / 32));

	
}





