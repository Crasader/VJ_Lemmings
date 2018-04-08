#include "EntityManager.h"
#include <iostream>


EntityManager::EntityManager(int numLemmings, ShaderProgram &shaderProgram, VariableTexture *map, VariableTexture *mask) {
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	this->mask = mask;
	this->map = map;
	bomb = NULL;
}

EntityManager::~EntityManager() {
}

void EntityManager::init() {
	elapsedTime = 0;
	timeToDisplay = 5;
	countingDown = false;
	offsetX = offsetY = 0;
	if (!countdown.init("fonts/upheavtt.ttf")) std::cout << "could not load font" << endl;
	spritesheet.loadFromFile("images/lemmings_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sceneTime = 0;
	lastLemmingCreation = 0;
	doubleSpeed = false;
	paused = false;
	spawnFrequency = 0;
	armageddon = false;
	lemmingsSaved = lemmingsDied = 0;

}

void EntityManager::update(int deltaTime, int buttonPressed,int offsetX, int offsetY){
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	sceneTime += deltaTime;
	elapsedTime += deltaTime;
	if (countingDown && elapsedTime >= 1000) {
		elapsedTime = 0;
		timeToDisplay--;
	}
	if (timeToDisplay <= -1) {
		killAllLemmings2();
		countingDown = false;
	}
	
	if ((sceneTime - lastLemmingCreation > (spawnTime + spawnFrequency)/2 && (numLemmings > 0)) && !paused && !armageddon && buttonPressed == 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(new Lemming());
		lemmings[lemmings.size() - 1]->init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1]->doubleSpeed();
	}
	else if ((sceneTime - lastLemmingCreation > spawnTime + spawnFrequency && (numLemmings > 0)) && !paused && !armageddon && buttonPressed != 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(new Lemming());
		lemmings[lemmings.size() - 1]->init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1]->doubleSpeed();
	}

	checkStatusLemmings();

	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (lemmings[i]->getStatus() == Lemming::DEAD_STATUS) {
			lemmings.erase(lemmings.begin() + i);
			lemmingsDied++;
		}
		else if (lemmings[i]->getStatus() == Lemming::EXITED_STATUS) {
			lemmings.erase(lemmings.begin() + i);
			lemmingsSaved++;
		}
	}
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->update(deltaTime);
		
	}

	this->buttonPressed = buttonPressed;
	doorStart->update(deltaTime);
	doorEnd->update(deltaTime);
	if (bomb != NULL) {
		bomb->update(deltaTime);
		bomb->changeState();
	}

}

void EntityManager::render() {
	doorStart->render();
	doorEnd->render();
	if (bomb != NULL) bomb->render();
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->render();
			
	}
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (countingDown) {
			glm::vec2 lemmingPos = lemmings[i]->getPosition();
			int lemPosX = lemmingPos.x * 3 - this->offsetX * 3 + 16;
			int lemPosY = lemmingPos.y * 3 + 15 - this->offsetY;
			countdown.render(to_string(timeToDisplay+1), glm::vec2(lemPosX , lemPosY ), 15, colorWhite);
		}
	}
}

void EntityManager::changeLemmingState(int x) {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->changeState(x);
	}
	x = 0;

}

bool EntityManager::clickManager(int mouseX, int mouseY, Effect state) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i]->getPosition(), mouseX, mouseY)) {
			if (!lemmingHasActionAssigned(i, state)) {
				lemmings[i]->changeState(state);
				return true;
			}
			break;
		}
	}
	return false;

}

bool EntityManager::lemmingInCursor(int mouseX, int mouseY) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i]->getPosition(), mouseX, mouseY)) {
			return true;
		}
	}
	return false;
}

bool EntityManager::checkCollision(glm::vec2 lemmingTopLeftPos, int mouseX, int mouseY) {

	if (mouseX >= lemmingTopLeftPos.x  && mouseX <= (lemmingTopLeftPos.x + 16) && mouseY >= lemmingTopLeftPos.y && mouseY <= (lemmingTopLeftPos.y + 16)) return true;
	else return false;

}

void EntityManager::doubleSpeedAnimation()
{
	doubleSpeed = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->doubleSpeed();
	}

	doorStart->doubleSpeed();
	doorEnd->doubleSpeed();
	if (bomb != NULL)bomb->doubleSpeed();
}

void EntityManager::resetNormalSpeed()
{
	doubleSpeed = false;
	paused = false;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->resetSpeed();
	}

	doorStart->resetSpeed();
	doorEnd->resetSpeed();
	if (bomb != NULL)bomb->resetSpeed();
}

void EntityManager::pause() {
	paused = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->pause();
	}

	doorStart->pause();
	doorEnd->pause();
	if (bomb != NULL)bomb->pause();
}

void EntityManager::increaseSpawnTime(){
	if(spawnFrequency < 4250)spawnFrequency += 85;
}

void EntityManager::decreaseSpawnTime(){
	if (spawnFrequency > -4250) spawnFrequency -= 85;
}

void EntityManager::killAllLemmings() {
	countingDown = true;
	armageddon = true;
}

void EntityManager::killAllLemmings2() {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if(countingDown )lemmings[i]->changeState(ARMAGEDDON_EFFECT);
	}
}

int EntityManager::getLemmingsExited()
{
	return lemmingsSaved;
}

int EntityManager::getLemmingsDied()
{
	return lemmingsDied;
}

void EntityManager::setStartDoor(glm::vec2 doorStartPosition, int doorStartType) {
	this->doorStartPosition = doorStartPosition;
	spritesheetStart.loadFromFile("images/start_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(doorStartType);
	doorStart->init(doorStartPosition, shaderProgram, spritesheetStart);

}

void EntityManager::setEndDoor(glm::vec2 doorEndPosition, int doorEndType) {
	this->doorEndPosition = doorEndPosition;
	spritesheetEnd.loadFromFile("images/end_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetEnd.setMinFilter(GL_NEAREST);
	spritesheetEnd.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(doorEndType);
	doorEnd->init(doorEndPosition, shaderProgram, spritesheetEnd);
}
void EntityManager::setBomb(glm::vec2 bombPosition) {
	spritesheetBomb.loadFromFile("images/bomb_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetBomb.setMinFilter(GL_NEAREST);
	spritesheetBomb.setMagFilter(GL_NEAREST);
	bomb = new Bomb();
	bomb->init(bombPosition, shaderProgram, spritesheetBomb, mask);
}
void EntityManager::checkStatusLemmings() {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		glm::vec2 posBase = lemmings[i]->getPosition() + glm::vec2(7, 16);
		if (posBase.x < 0 || posBase.x > mask->width() || posBase.y < 0 || posBase.y > 160)
			lemmings[i]->goKill();
		glm::vec2 exitBase = doorEndPosition + glm::vec2(18, 32);
		if (posBase.x == exitBase.x && posBase.y == exitBase.y)
			lemmings[i]->goExit();
	}
}

bool EntityManager::lemmingHasActionAssigned(int i, Effect state) {
	if (state == DIGGER_EFFECT && (lemmings[i]->getState() == Lemming::DIGGER_STATE || lemmings[i]->getNextState() == Lemming::DIGGER_TRIGGERED)) return true;
	else if (state == BLOCKER_EFFECT && (lemmings[i]->getState() == Lemming::BLOCKER_STATE || lemmings[i]->getNextState() == Lemming::BLOCKER_TRIGGERED)) return true;
	else if (state == BASHER_EFFECT && (lemmings[i]->getState() == Lemming::BASHER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::BASHER_TRIGGERED || lemmings[i]->getState() == Lemming::BASHER_RIGHT_STATE)) return true;
	else if (state == CLIMBER_EFFECT && (lemmings[i]->getState() == Lemming::CLIMBER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::CLIMBER_TRIGGERED || lemmings[i]->getState() == Lemming::CLIMBER_RIGHT_STATE)) return true;
	else if (state == BUILDER_EFFECT && (lemmings[i]->getState() == Lemming::BUILDER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::BUILDER_TRIGGERED || lemmings[i]->getState() == Lemming::BUILDER_RIGHT_STATE)) return true;
	else if (state == FLOATER_EFFECT && (lemmings[i]->getState() == Lemming::FLOATER_LEFT_STATE || lemmings[i]->getState() == Lemming::FLOATER_RIGHT_STATE || lemmings[i]->getNextState() == Lemming::FLOATER_TRIGGERED)) return true;
	else if (state == BOMBER_EFFECT && (lemmings[i]->getState() == Lemming::BOMBER_LEFT_STATE || lemmings[i]->getState() == Lemming::BOMBER_RIGHT_STATE || lemmings[i]->getNextState() == Lemming::BOMBER_TRIGGERED)) return true;
	else return false;
}