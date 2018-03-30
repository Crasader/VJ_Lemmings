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


EntityManager::~EntityManager()
{
}

void EntityManager::update(int deltaTime){
	sceneTime += deltaTime;
	if ((sceneTime - lastLemmingCreation > spawnTime + spawnFrequency && (numLemmings > 0)) && !paused) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(Lemming());
		lemmings[lemmings.size()-1].init(doorStartPosition, shaderProgram, spritesheet,mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1].doubleSpeed();
		else if (paused) lemmings[lemmings.size() - 1].pause();
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

void EntityManager::init() {
	spritesheet.loadFromFile("images/lemmings_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sceneTime = 0;
	lastLemmingCreation = 0;
	lemmings.push_back(Lemming());
	lemmings[0].init(doorStartPosition, shaderProgram,spritesheet,mask);
	numLemmings--;

	doubleSpeed = false;
	paused = false;
	spawnFrequency = 0;


	spritesheetStart.loadFromFile(dorIni, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(DoorStart::BROWN);
	doorStart->init(doorStartPosition, shaderProgram, spritesheetStart);

	spritesheetEnd.loadFromFile(dorEnd, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetEnd.setMinFilter(GL_NEAREST);
	spritesheetEnd.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(DoorEnd::BLACK);
	doorEnd->init(glm::vec2(260, 30), shaderProgram, spritesheetEnd);
	
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

	if (mouseX >= lemmingTopLeftPos.x  && mouseX <= (lemmingTopLeftPos.x + 16) && mouseY >= lemmingTopLeftPos.y && mouseY <= (lemmingTopLeftPos.y + 16)) return true;
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

