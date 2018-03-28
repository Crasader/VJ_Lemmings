#include "EntityManager.h"


EntityManager::EntityManager(int numLemmings, glm::vec2 &doorPosition, ShaderProgram &shaderProgram,VariableTexture *mask) {
	this->doorPosition = doorPosition;
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	this->mask = mask;
	init();
}


EntityManager::~EntityManager()
{
}

void EntityManager::update(int deltaTime){
	sceneTime += deltaTime;
	if (sceneTime - lastLemmingCreation > spawnTime && (numLemmings > 0)) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(Lemming());
		lemmings[lemmings.size()-1].init(doorPosition, shaderProgram, spritesheet,mask);
	}
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i].update(deltaTime);
	}


}

void EntityManager::render() {
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
	lemmings[0].init(doorPosition, shaderProgram,spritesheet,mask);
	numLemmings--;
	
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

