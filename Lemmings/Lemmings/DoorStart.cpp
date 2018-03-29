#include "DoorStart.h"



DoorStart::DoorStart(int color) {
	if (BROWN) {
		ySpriteSheet = 0;
	}
	else {
		ySpriteSheet = 0.5;
	}
}


DoorStart::~DoorStart() {
}

void DoorStart::init(const glm::vec2 & initialPosition, ShaderProgram & shaderProgram, Texture & spritesheet) {
	this->shaderProgram = shaderProgram;

	sprite = Sprite::createSprite(glm::ivec2(48, 30), glm::vec2(0.1, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);
	setAnimations();

	sprite->changeAnimation(CLOSED);
	sprite->setPosition(initialPosition);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void DoorStart::update(int deltaTime) {
}

void DoorStart::render() {
	glm::mat4 modelview;
	shaderProgram.use();
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	shaderProgram.setUniformMatrix4f("modelview", modelview);
	sprite->render();
}

void DoorStart::setAnimations() {

	sprite->setAnimationSpeed(CLOSED, 0);
	sprite->addKeyframe(CLOSED, glm::vec2(0, ySpriteSheet));

	sprite->setAnimationSpeed(OPENING, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(OPENING, glm::vec2(0.1 + (float(i) / 10), ySpriteSheet));

	sprite->setAnimationSpeed(OPEN, 0);
	sprite->addKeyframe(OPEN, glm::vec2(0.9, ySpriteSheet));
}


glm::vec2 DoorStart::getPosition() {
	return sprite->position();
}