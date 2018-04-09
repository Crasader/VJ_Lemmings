#include "Bomb.h"
#include <cmath>
#include <algorithm>
#include "AudioEngine.h"


Bomb::Bomb() {
}


Bomb::~Bomb() {
}

void Bomb::init(const glm::vec2 initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *mask) {
	setMask(mask);
	state = IDLE_STATE;
	actionTime = 0;

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	setAnimations();

	sprite->changeAnimation(IDLE);
	sprite->setPosition(initialPosition);
}


void Bomb::update(int deltaTime) {
	currentTime += deltaTime;
	if (sprite->update(deltaTime) == 0)
		return;

	actionTime++;
}

void Bomb::render() {
	sprite->render();
}

void Bomb::changeState() {
	switch (state) {
	case DROPPED_STATE:
		if (actionTime > 8) goTrigger();
		break;
	case EXPLODING_STATE:
		if (actionTime > 16) goExplode();
		break;
	case EXPLODED_STATE:
		if (actionTime > 7) goEnd();
		break;
	default:
		break;
	}
}

Bomb::BombState Bomb::getState() {
	return state;
}

void Bomb::setPosition(glm::vec2 newPosition) {
	sprite->position() = newPosition;
}

void Bomb::goPicked() {
	state = PICKED_STATE;
}

void Bomb::goDrop() {
	resetActionTime();
	sprite->changeAnimation(DROPPED);
	state = DROPPED_STATE;
}

void Bomb::goTrigger() {
	resetActionTime();
	sprite->changeAnimation(GOING_TO_EXPLODE);
	state = EXPLODING_STATE;
}

void Bomb::goExplode() {
	AudioEngine::instance().explosionEffect();
	resetActionTime();
	explode();
	sprite->changeAnimation(EXPLODING);
	state = EXPLODED_STATE;
}

void Bomb::goEnd() {
	resetActionTime();
	state = END_STATE;
}

void Bomb::doubleSpeed() {
	sprite->setAnimationSpeed(IDLE, speed * 2);
	sprite->setAnimationSpeed(DROPPED, speed * 2);
	sprite->setAnimationSpeed(GOING_TO_EXPLODE, speed * 2);
	sprite->setAnimationSpeed(EXPLODING, speed * 2);
}

void Bomb::resetSpeed() {
	sprite->setAnimationSpeed(IDLE, speed);
	sprite->setAnimationSpeed(DROPPED, speed);
	sprite->setAnimationSpeed(GOING_TO_EXPLODE, speed);
	sprite->setAnimationSpeed(EXPLODING, speed);
}

void Bomb::pause() {
	sprite->setAnimationSpeed(IDLE, 0);
	sprite->setAnimationSpeed(DROPPED, 0);
	sprite->setAnimationSpeed(GOING_TO_EXPLODE, 0);
	sprite->setAnimationSpeed(EXPLODING, 0);
}

void Bomb::explode() {
	glm::ivec2 position = sprite->position();
	deleteMask(position.x + 6, position.x + 9, position.y + 0);
	deleteMask(position.x + 3, position.x + 12, position.y + 1);
	deleteMask(position.x + 1, position.x + 14, position.y + 2);
	deleteMask(position.x + 0, position.x + 15, position.y + 3);
	deleteMask(position.x + -1, position.x + 16, position.y + 4);
	deleteMask(position.x + -2, position.x + 17, position.y + 5);
	deleteMask(position.x + -2, position.x + 17, position.y + 6);
	deleteMask(position.x + -3, position.x + 18, position.y + 7);
	deleteMask(position.x + -3, position.x + 18, position.y + 8);
	deleteMask(position.x + -3, position.x + 18, position.y + 9);
	deleteMask(position.x + -4, position.x + 19, position.y + 10);
	deleteMask(position.x + -4, position.x + 19, position.y + 11);
	deleteMask(position.x + -4, position.x + 19, position.y + 12);
	deleteMask(position.x + -4, position.x + 19, position.y + 13);
	deleteMask(position.x + -3, position.x + 18, position.y + 14);
	deleteMask(position.x + -3, position.x + 18, position.y + 15);
	deleteMask(position.x + -3, position.x + 18, position.y + 16);
	deleteMask(position.x + -2, position.x + 17, position.y + 17);
	deleteMask(position.x + -2, position.x + 17, position.y + 18);
	deleteMask(position.x + -1, position.x + 16, position.y + 19);
	deleteMask(position.x + 0, position.x + 15, position.y + 20);
	deleteMask(position.x + 1, position.x + 14, position.y + 21);
	deleteMask(position.x + 3, position.x + 12, position.y + 22);
	deleteMask(position.x + 6, position.x + 9, position.y + 23);
}

void Bomb::deleteMask(int xStart, int xEnd, int y) {
	if (y > 0 && y < mask->height())
		for (int x = max(0, xStart); x <= min(mask->width(), xEnd); x++)
			mask->setPixel(x, y, 0);
}

void Bomb::setMask(VariableTexture * mapMask) {
	mask = mapMask;
}

void Bomb::setAnimations() {
	sprite->setAnimationSpeed(IDLE, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(IDLE, glm::vec2(float(i) / 8, 0));

	sprite->setAnimationSpeed(DROPPED, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(DROPPED, glm::vec2(float(i) / 8, 1.f / 5));

	sprite->setAnimationSpeed(GOING_TO_EXPLODE, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(GOING_TO_EXPLODE, glm::vec2(float(i) / 8, 2.f / 5));
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(GOING_TO_EXPLODE, glm::vec2(float(i) / 8, 3.f / 5));


	sprite->setAnimationSpeed(EXPLODING, speed);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(EXPLODING, glm::vec2(float(i) / 8, 4.f / 5));
}

void Bomb::resetActionTime() {
	actionTime = 0;
}
