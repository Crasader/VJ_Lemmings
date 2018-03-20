#include "EntityManager.h"


EntityManager::EntityManager(int numLemmings, glm::vec2 &doorPosition, ShaderProgram &shaderProgram) {
	this->doorPosition = doorPosition;
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	init();
}


EntityManager::~EntityManager()
{
}

void EntityManager::update(int deltaTime){
	sceneTime += deltaTime;
	if (sceneTime - lastLemmingCreation > spawnTime) {
		lemmings.push_back(Lemming());
		lemmings[lemmings.size()-1].init(doorPosition, shaderProgram, spritesheet);
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
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sceneTime = 0;
	lastLemmingCreation = 0;
	lemmings.push_back(Lemming());
	lemmings[0].init(doorPosition, shaderProgram,spritesheet);
	numLemmings--;
	
}

