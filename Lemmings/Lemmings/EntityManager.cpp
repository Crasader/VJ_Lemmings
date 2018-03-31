#include "EntityManager.h"


EntityManager::EntityManager(int numLemmings, glm::vec2 &doorStartPosition, glm::vec2 &doorEndPosition, ShaderProgram &shaderProgram,VariableTexture *mask, string dorIni, string dorEnd) {
	this->doorStartPosition = doorStartPosition;
	this->doorEndPosition = doorEndPosition;
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	this->mask = mask;
	this->dorIni = dorIni;
	this->dorEnd = dorEnd;
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
	lemmings.push_back(Lemming());
	lemmings[0].init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, mask);
	numLemmings--;

	doubleSpeed = false;
	paused = false;
	spawnFrequency = 0;
	armagedon = false;

	spritesheetStart.loadFromFile(dorIni, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(DoorStart::BROWN);
	doorStart->init(doorStartPosition, shaderProgram, spritesheetStart);

	spritesheetEnd.loadFromFile(dorEnd, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetEnd.setMinFilter(GL_NEAREST);
	spritesheetEnd.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(DoorEnd::BLACK);
	doorEnd->init(doorEndPosition, shaderProgram, spritesheetEnd);
}

void EntityManager::update(int deltaTime){
	sceneTime += deltaTime;
	if ((sceneTime - lastLemmingCreation > spawnTime + spawnFrequency && (numLemmings > 0)) && !paused && !armagedon) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(Lemming());
		lemmings[lemmings.size()-1].init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet,mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1].doubleSpeed();
		//else if (paused) lemmings[lemmings.size() - 1].pause();
	}

	checkMapLimits();

	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (lemmings[i].getStatus() == Lemming::DEAD_STATUS) lemmings.erase(lemmings.begin() + i);
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

void EntityManager::clickManager(int mouseX, int mouseY, int state) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i].getPosition(), mouseX, mouseY)) {
			lemmings[i].changeState(state);
			break;
		}
	}

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
	for (int i = 0; i < lemmings.size(); ++i) {
		lemmings[i].changeState(8);
	}
}

void EntityManager::checkMapLimits() {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		glm::vec2 posBase = lemmings[i].getPosition() + glm::vec2(7, 16);
		if (posBase.x < 0 || posBase.x > mask->width() || posBase.y < 0 || posBase.y > mask->height())
			lemmings[i].kill();
	}
}