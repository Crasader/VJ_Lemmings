#include "EntityManager.h"


EntityManager::EntityManager(int numLemmings, glm::vec2 &doorStartPosition, int doorStartType, glm::vec2 &doorEndPosition, int doorEndType, ShaderProgram &shaderProgram, VariableTexture *map, VariableTexture *mask, string dorIni, string dorEnd) {
	this->doorStartPosition = doorStartPosition;
	this->doorEndPosition = doorEndPosition;
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	this->mask = mask;
	this->map = map;
	this->dorIni = dorIni;
	this->dorEnd = dorEnd;
	this->doorSColor = doorStartType;
	this->doorEColor = doorEndType;
	init();
}

EntityManager::~EntityManager() {
}

void EntityManager::init() {
	spritesheet.loadFromFile("images/lemmings_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sceneTime = 0;
	lastLemmingCreation = 0;
	doubleSpeed = false;
	paused = false;
	spawnFrequency = 0;
	armagedon = false;
	lemmingsSaved = 0;

	spritesheetStart.loadFromFile(dorIni, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(doorSColor);
	doorStart->init(doorStartPosition, shaderProgram, spritesheetStart);

	spritesheetEnd.loadFromFile(dorEnd, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetEnd.setMinFilter(GL_NEAREST);
	spritesheetEnd.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(doorEColor);
	doorEnd->init(doorEndPosition, shaderProgram, spritesheetEnd);
}

void EntityManager::update(int deltaTime, int buttonPressed){
	sceneTime += deltaTime;
	
	if ((sceneTime - lastLemmingCreation > (spawnTime + spawnFrequency)/2 && (numLemmings > 0)) && !paused && !armagedon && buttonPressed == 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(Lemming());
		lemmings[lemmings.size() - 1].init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1].doubleSpeed();
	}
	else if ((sceneTime - lastLemmingCreation > spawnTime + spawnFrequency && (numLemmings > 0)) && !paused && !armagedon && buttonPressed != 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(Lemming());
		lemmings[lemmings.size() - 1].init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1].doubleSpeed();
	}

	checkStatusLemmings();

	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (lemmings[i].getStatus() == Lemming::DEAD_STATUS) lemmings.erase(lemmings.begin() + i);
		else if (lemmings[i].getStatus() == Lemming::EXITED_STATUS) {
			lemmings.erase(lemmings.begin() + i);
			lemmingsSaved++;
		}
	}
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].update(deltaTime);
	}

	this->buttonPressed = buttonPressed;
	doorStart->update(deltaTime);
	doorEnd->update(deltaTime);

}

void EntityManager::render() {
	doorStart->render();
	doorEnd->render();
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].render();
	}
}

void EntityManager::changeLemmingState(int x) {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].changeState(x);
	}
	x = 0;

}

bool EntityManager::clickManager(int mouseX, int mouseY, int state) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i].getPosition(), mouseX, mouseY) && !lemmingHasActionAssigned(i)) {
			lemmings[i].changeState(state);
			return true;
		}
	}
	return false;

}

bool EntityManager::lemmingInCursor(int mouseX, int mouseY) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i].getPosition(), mouseX, mouseY)) {
			return true;
		}
	}
	return false;
}

bool EntityManager::checkCollision(glm::vec2 lemmingTopLeftPos, int mouseX, int mouseY) {

	if (mouseX >= lemmingTopLeftPos.x  && mouseX <= (lemmingTopLeftPos.x + 16) && mouseY >= lemmingTopLeftPos.y && mouseY <= (lemmingTopLeftPos.y + 20)) return true;
	else return false;

}

void EntityManager::doubleSpeedAnimation()
{
	doubleSpeed = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].doubleSpeed();
	}

	doorStart->doubleSpeed();
	doorEnd->doubleSpeed();
}

void EntityManager::resetNormalSpeed()
{
	doubleSpeed = false;
	paused = false;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].resetSpeed();
	}

	doorStart->resetSpeed();
	doorEnd->resetSpeed();
}

void EntityManager::pause() {
	paused = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].pause();
	}

	doorStart->pause();
	doorEnd->pause();
}

void EntityManager::increaseSpawnTime(){
	if(spawnFrequency < 4250)spawnFrequency += 85;
}

void EntityManager::decreaseSpawnTime(){
	if (spawnFrequency > -4250) spawnFrequency -= 85;
}

void EntityManager::killAllLemmings() {
	armagedon = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].changeState(8);
	}
}

int EntityManager::getLemmingsSaved()
{
	return lemmingsSaved;
}

void EntityManager::checkStatusLemmings() {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		glm::vec2 posBase = lemmings[i].getPosition() + glm::vec2(7, 16);
		if (posBase.x < 0 || posBase.x > mask->width() || posBase.y < 0 || posBase.y > 160)
			lemmings[i].goKill();
		glm::vec2 exitBase = doorEndPosition + glm::vec2(18, 32);
		if (posBase.x == exitBase.x && posBase.y == exitBase.y)
			lemmings[i].goExit();
	}
}

bool EntityManager::lemmingHasActionAssigned(int i) {
	if (lemmings[i].getState() == Lemming::BASHER_LEFT_STATE ||  lemmings[i].getState() == Lemming::BASHER_RIGHT_STATE || lemmings[i].getState() == Lemming::BLOCKER_STATE ||
		lemmings[i].getState() == Lemming::BOMBER_LEFT_STATE || lemmings[i].getState() == Lemming::BUILDER_LEFT_STATE || lemmings[i].getState() == Lemming::BOMBER_RIGHT_STATE || lemmings[i].getState() == Lemming::BUILDER_RIGHT_STATE ||
		lemmings[i].getState() == Lemming::CLIMBER_LEFT_STATE || lemmings[i].getState() == Lemming::DIGGER_STATE || lemmings[i].getState() == Lemming::CLIMBER_RIGHT_STATE ||
		lemmings[i].getState() == Lemming::FLOATER_LEFT_STATE || lemmings[i].getState() == Lemming::FLOATER_RIGHT_STATE) return true;
	else if(lemmings[i].getNextState() == Lemming::BASHER_TRIGGERED || lemmings[i].getNextState() == Lemming::BLOCKER_TRIGGERED ||
		lemmings[i].getNextState() == Lemming::BOMBER_TRIGGERED || lemmings[i].getNextState() == Lemming::BUILDER_TRIGGERED ||
		lemmings[i].getNextState() == Lemming::CLIMBER_TRIGGERED || lemmings[i].getNextState() == Lemming::DIGGER_TRIGGERED ||
		lemmings[i].getNextState() == Lemming::FLOATER_TRIGGERED)return true;
	else return false;
}