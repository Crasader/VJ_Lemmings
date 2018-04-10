#include "Portal.h"



Portal::Portal(PortalColors color) {
	if (color == BLUE)
		ySpriteSheet = 0.f;
	else
		ySpriteSheet = 0.5f;
}


Portal::~Portal() {
}

void Portal::init(const glm::vec2 & initialPosition, ShaderProgram & shaderProgram, Texture & spritesheet) {
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(0, glm::vec2(float(i) / 8, ySpriteSheet));

	currentTime = 0.f;
	sprite->changeAnimation(0);
	sprite->setPosition(initialPosition);
}

void Portal::update(int deltaTime) {
	currentTime += deltaTime;

	if (sprite->update(deltaTime) == 0)
		return;
}

void Portal::render() {
	sprite->render();
}

void Portal::doubleSpeed() {
	sprite->setAnimationSpeed(0, speed * 2);
}

void Portal::pause() {
	sprite->setAnimationSpeed(0, 0);
}

void Portal::resetSpeed() {
	sprite->setAnimationSpeed(0, speed);
}
