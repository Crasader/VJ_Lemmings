#include "DoorEnd.h"



DoorEnd::DoorEnd(int color) {
	if (color == BLACK) {
		xSpriteSheet = 0.f;
	}
	else if (color == BROWN) {
		xSpriteSheet = 0.25f;
	}
	else if (color == PINK) {
		xSpriteSheet = 0.5f;
	}
	else {
		xSpriteSheet = 0.75f;
	}
}


DoorEnd::~DoorEnd() {
}

void DoorEnd::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet) {


	sprite = Sprite::createSprite(glm::ivec2(42, 32), glm::vec2(0.25f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, speedDoorEnd);
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(0, glm::vec2(xSpriteSheet, float(i) / 8));

	sprite->changeAnimation(0);
	sprite->setPosition(initialPosition);
}

void DoorEnd::update(int deltaTime) {

	if (sprite->update(deltaTime) == 0)
		return;

}

void DoorEnd::render() {
	sprite->render();
}

glm::vec2 DoorEnd::getPosition() {
	return sprite->position();
}

void DoorEnd::pause() {
	sprite->setAnimationSpeed(0, 0);
}

void DoorEnd::doubleSpeed() {
	sprite->setAnimationSpeed(0, speedDoorEnd*2);
}

void DoorEnd::resetSpeed() {
	sprite->setAnimationSpeed(0, speedDoorEnd);
}