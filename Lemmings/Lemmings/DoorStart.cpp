#include "DoorStart.h"



DoorStart::DoorStart(int color) {
	if (color == BROWN) {
		xSpriteSheet = 0.f;
	}
	else {
		xSpriteSheet = 0.5f;
	}
}


DoorStart::~DoorStart() {
}

void DoorStart::init(const glm::vec2 & initialPosition, ShaderProgram & shaderProgram, Texture & spritesheet) {

	sprite = Sprite::createSprite(glm::ivec2(48, 30), glm::vec2(0.5f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
	setAnimations();

	sprite->changeAnimation(CLOSED);
	sprite->setPosition(initialPosition);
	
}

void DoorStart::update(int deltaTime) {

	if (sprite->update(deltaTime) == 0)
		return;

}

void DoorStart::render() {
	sprite->render();
}

void DoorStart::setAnimations() {

	sprite->setAnimationSpeed(CLOSED, 0);
	sprite->addKeyframe(CLOSED, glm::vec2(xSpriteSheet, 0.f));

	sprite->setAnimationSpeed(OPENING, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(OPENING, glm::vec2(xSpriteSheet, 0.1 + (float(i) / 10)));

	sprite->setAnimationSpeed(OPEN, 0);
	sprite->addKeyframe(OPEN, glm::vec2(xSpriteSheet, 0.9f));
}


glm::vec2 DoorStart::getPosition() {
	return sprite->position();
}