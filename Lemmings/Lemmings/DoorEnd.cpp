#include "DoorEnd.h"



DoorEnd::DoorEnd(int color) {
	if (BLACK) {
		ySpriteSheet = 0.f;
	}
	else if (BROWN) {
		ySpriteSheet = 0.25f;
	}
	else if (PINK) {
		ySpriteSheet = 0.5f;
	}
	else {
		ySpriteSheet = 0.75f;
	}
}


DoorEnd::~DoorEnd() {
}

void DoorEnd::init(const glm::vec2 & initialPosition, ShaderProgram & shaderProgram, Texture & spritesheet) {
	sprite = Sprite::createSprite(glm::ivec2(42, 32), glm::vec2(1/6, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, speed);
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(0, glm::vec2((float(i) / 6), ySpriteSheet));

	sprite->changeAnimation(0);
	sprite->setPosition(initialPosition);
}

void DoorEnd::update(int deltaTime) {
}

void DoorEnd::render() {
	sprite->render();
}

glm::vec2 DoorEnd::getPosition() {
	return sprite->position();
}
