#include "EntityManager.h"
#include <iostream>
#include "AudioEngine.h"


EntityManager::EntityManager(int numLemmings, ShaderProgram &shaderProgram, VariableTexture *map, VariableTexture *mask) {
	this->shaderProgram = shaderProgram;
	this->numLemmings = numLemmings;
	this->mask = mask;
	this->map = map;
	bomb = NULL;
	portalBlue = NULL;
	portalOrange = NULL;
}

EntityManager::~EntityManager() {
}

void EntityManager::init() {
	elapsedTime = 0;
	timeToDisplay = 5;
	countingDown = false;
	offsetX = offsetY = 0;
	if (!countdown.init("fonts/upheavtt.ttf")) std::cout << "could not load font" << endl;
	spritesheet.loadFromFile("images/lemmings_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sceneTime = 0;
	lastLemmingCreation = 0;
	doubleSpeed = false;
	paused = false;
	spawnFrequency = 0;
	armageddon = false;
	lemmingsSaved = lemmingsDied = 0;
	bombersAmount = 0;

}

void EntityManager::update(int deltaTime, int buttonPressed,int offsetX, int offsetY){
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	sceneTime += deltaTime;
	elapsedTime += deltaTime;
	if (countingDown && elapsedTime >= 1000) {
		elapsedTime = 0;
		timeToDisplay--;
	}
	if (timeToDisplay <= -1) {
		killAllLemmings2();
		countingDown = false;
	}
	
	if ((sceneTime - lastLemmingCreation > (spawnTime + spawnFrequency)/2 && (numLemmings > 0)) && !paused && !armageddon && buttonPressed == 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(new Lemming());
		lemmings[lemmings.size() - 1]->init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		if (doubleSpeed) lemmings[lemmings.size() - 1]->doubleSpeed();
	}
	else if ((sceneTime - lastLemmingCreation > spawnTime + spawnFrequency && (numLemmings > 0)) && !paused && !armageddon && buttonPressed != 9) {
		lastLemmingCreation = sceneTime;
		numLemmings--;
		lemmings.push_back(new Lemming());
		lemmings[lemmings.size() - 1]->init(doorStartPosition + glm::vec2(16, 0), shaderProgram, spritesheet, map, mask);
		fireworks.push_back(new Firework());
		
		if (doubleSpeed) lemmings[lemmings.size() - 1]->doubleSpeed();
	}

	checkStatusLemmings();


	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->update(deltaTime);
		
	}

	this->buttonPressed = buttonPressed;
	doorStart->update(deltaTime);
	doorEnd->update(deltaTime);
	if (portalBlue != NULL)
		portalBlue->update(deltaTime);
	if (portalOrange != NULL)
		portalOrange->update(deltaTime);
	if (bomb != NULL && (bomb->getState() != Bomb::PICKED_STATE && bomb->getState() != Bomb::END_STATE)) {
		bomb->update(deltaTime);
		bomb->changeState();
	}
	

}

void EntityManager::render() {
	doorStart->render();
	doorEnd->render();
	if (portalBlue != NULL)
		portalBlue->render();
	if (portalOrange != NULL)
		portalOrange->render();
	
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->render();
			
	}
	if (bomb != NULL && (bomb->getState() != Bomb::PICKED_STATE && bomb->getState() != Bomb::END_STATE)) bomb->render();
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (countingDown) {
			glm::vec2 lemmingPos = lemmings[i]->getPosition();
			int lemPosX = lemmingPos.x * 3 - this->offsetX * 3 + 16;
			int lemPosY = lemmingPos.y * 3 + 15 - this->offsetY;
			countdown.render(to_string(timeToDisplay+1), glm::vec2(lemPosX , lemPosY ), 15, colorWhite);
			
		}
	}
	for (int i = 0; i < (int)fireworks.size(); ++i)
		fireworks[i]->render();
}

// TODO esta función no se utiliza debería borrarse
void EntityManager::changeLemmingState(int x) {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->changeState(x);
	}
	x = 0;

}

bool EntityManager::clickManager(int mouseX, int mouseY, Effect state) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i]->getPosition(), mouseX, mouseY)) {
			if (!lemmingHasActionAssigned(i, state)) {
				lemmings[i]->changeState(state);
				if (state == BOMBER_EFFECT) // bomber
					dropBomb(lemmings[i]->getPosition());
				return true;
			}
			break;
		}
	}
	return false;

}

bool EntityManager::lemmingInCursor(int mouseX, int mouseY) {
	for (int i = lemmings.size() - 1; i >= 0; i--) {
		if (checkCollision(lemmings[i]->getPosition(), mouseX, mouseY)) {
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
		lemmings[i]->doubleSpeed();
	}

	doorStart->doubleSpeed();
	doorEnd->doubleSpeed();
	if (portalBlue != NULL || portalOrange != NULL) {
		portalBlue->doubleSpeed();
		portalOrange->doubleSpeed();
	}
	if (bomb != NULL && (bomb->getState() != Bomb::PICKED_STATE && bomb->getState() != Bomb::END_STATE))bomb->doubleSpeed();
}

void EntityManager::resetNormalSpeed()
{
	doubleSpeed = false;
	paused = false;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->resetSpeed();
	}

	doorStart->resetSpeed();
	doorEnd->resetSpeed();
	if (portalBlue != NULL || portalOrange != NULL) {
		portalBlue->resetSpeed();
		portalOrange->resetSpeed();
	}
	if (bomb != NULL && (bomb->getState() != Bomb::PICKED_STATE && bomb->getState() != Bomb::END_STATE))bomb->resetSpeed();
}

void EntityManager::pause() {
	paused = true;
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		lemmings[i]->pause();
	}

	doorStart->pause();
	doorEnd->pause();
	if (portalBlue != NULL || portalOrange != NULL) {
		portalBlue->pause();
		portalOrange->pause();
	}
	if (bomb != NULL && (bomb->getState() != Bomb::PICKED_STATE && bomb->getState() != Bomb::END_STATE))bomb->pause();
}

void EntityManager::increaseSpawnTime(){
	if(spawnFrequency < 4250)spawnFrequency += 85;
}

void EntityManager::decreaseSpawnTime(){
	if (spawnFrequency > -4250) spawnFrequency -= 85;
}

void EntityManager::killAllLemmings() {
	countingDown = true;
	armageddon = true;
}

void EntityManager::killAllLemmings2() {
	
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if(countingDown)lemmings[i]->changeState(ARMAGEDDON_EFFECT);
	}
}

int EntityManager::getLemmingsExited()
{
	return lemmingsSaved;
}

int EntityManager::getLemmingsDied()
{
	return lemmingsDied;
}

int EntityManager::getBombersAmount()
{
	return bombersAmount;
}

void EntityManager::setStartDoor(glm::vec2 doorStartPosition, int doorStartType) {
	this->doorStartPosition = doorStartPosition;
	spritesheetStart.loadFromFile("images/start_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(doorStartType);
	doorStart->init(doorStartPosition, shaderProgram, spritesheetStart);

}

void EntityManager::setEndDoor(glm::vec2 doorEndPosition, int doorEndType) {
	this->doorEndPosition = doorEndPosition;
	spritesheetEnd.loadFromFile("images/end_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetEnd.setMinFilter(GL_NEAREST);
	spritesheetEnd.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(doorEndType);
	doorEnd->init(doorEndPosition, shaderProgram, spritesheetEnd);
}

void EntityManager::setBomb(glm::vec2 bombPosition) {
	this->bombPosition = bombPosition;
	spritesheetBomb.loadFromFile("images/bomb_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetBomb.setMinFilter(GL_NEAREST);
	spritesheetBomb.setMagFilter(GL_NEAREST);
	bomb = new Bomb();
	bomb->init(bombPosition, shaderProgram, spritesheetBomb, mask);
}

void EntityManager::setPortals(glm::vec2 portalBluePosition, glm::vec2 portalOrangePosition) {
	this->portalBluePosition = portalBluePosition;
	this->portalOrangePosition = portalOrangePosition;
	spritesheetPortals.loadFromFile("images/portal_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetBomb.setMinFilter(GL_NEAREST);
	spritesheetBomb.setMagFilter(GL_NEAREST);
	portalBlue = new Portal(Portal::BLUE);
	portalBlue->init(portalBluePosition, shaderProgram, spritesheetPortals);
	portalOrange = new Portal(Portal::ORANGE);
	portalOrange->init(portalOrangePosition, shaderProgram, spritesheetPortals);
}

int EntityManager::getNumLemmingsOut() {
	return lemmings.size();
}

void EntityManager::dropBomb(glm::vec2 newPosition) {
	bombersAmount--;
	bomb->setPosition(newPosition);
	bomb->goDrop();
}

void EntityManager::checkStatusLemmings() {
	for (int i = 0; i < (int)lemmings.size(); ++i) {
		glm::vec2 posBase = lemmings[i]->getPosition() + glm::vec2(7, 16);
		if (posBase.x < 0 || posBase.x > mask->width() || posBase.y < 0 || posBase.y > 160)
			lemmings[i]->goKill();
		glm::vec2 exitBase = doorEndPosition + glm::vec2(18, 32);
		if (posBase.x == exitBase.x && posBase.y == exitBase.y)
			lemmings[i]->goExit();
		glm::vec2 bombBase = bombPosition + glm::vec2(0, 0);
		if (bomb != NULL && bomb->getState() == Bomb::IDLE_STATE && (posBase.x > bombBase.x && posBase.x < bombBase.x + 17) && (posBase.y > bombBase.y && posBase.y < bombBase.y + 17)) {
			bombersAmount++;
			bomb->goPicked();
		}
		glm::vec2 portalBlueBase = portalBluePosition + glm::vec2(12,31);
		if (portalBlue != NULL && posBase.x == portalBlueBase.x && posBase.y == portalBlueBase.y) {
			AudioEngine::instance().portalEffect();
			lemmings[i]->setPosition(portalOrangePosition + glm::vec2(8, 8));
		}
	}

	for (int i = 0; i < (int)lemmings.size(); ++i) {
		if (lemmings[i]->getStatus() == Lemming::DEAD_STATUS) {
			if (armageddon) startFireworks(i);
			lemmings.erase(lemmings.begin() + i);
			lemmingsDied++;
		}
		else if (lemmings[i]->getStatus() == Lemming::EXITED_STATUS) {
			lemmings.erase(lemmings.begin() + i);
			lemmingsSaved++;
		}
	}
}

bool EntityManager::lemmingHasActionAssigned(int i, Effect state) {
	if (state == DIGGER_EFFECT && (lemmings[i]->getState() == Lemming::DIGGER_STATE || lemmings[i]->getNextState() == Lemming::DIGGER_TRIGGERED)) return true;
	else if (state == BLOCKER_EFFECT && (lemmings[i]->getState() == Lemming::BLOCKER_STATE || lemmings[i]->getNextState() == Lemming::BLOCKER_TRIGGERED)) return true;
	else if (state == BASHER_EFFECT && (lemmings[i]->getState() == Lemming::BASHER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::BASHER_TRIGGERED || lemmings[i]->getState() == Lemming::BASHER_RIGHT_STATE)) return true;
	else if (state == CLIMBER_EFFECT && (lemmings[i]->getState() == Lemming::CLIMBER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::CLIMBER_TRIGGERED || lemmings[i]->getState() == Lemming::CLIMBER_RIGHT_STATE)) return true;
	else if (state == BUILDER_EFFECT && (lemmings[i]->getState() == Lemming::BUILDER_LEFT_STATE || lemmings[i]->getNextState() == Lemming::BUILDER_TRIGGERED || lemmings[i]->getState() == Lemming::BUILDER_RIGHT_STATE)) return true;
	else if (state == FLOATER_EFFECT && (lemmings[i]->getState() == Lemming::FLOATER_LEFT_STATE || lemmings[i]->getState() == Lemming::FLOATER_RIGHT_STATE || lemmings[i]->getNextState() == Lemming::FLOATER_TRIGGERED)) return true;
	else if (state == BOMBER_EFFECT && (lemmings[i]->getState() == Lemming::BOMBER_LEFT_STATE || lemmings[i]->getState() == Lemming::BOMBER_RIGHT_STATE || lemmings[i]->getNextState() == Lemming::BOMBER_TRIGGERED)) return true;
	else return false;
}

void EntityManager::startFireworks(int i) {
		glm::vec2 pos;
		pos = lemmings[i]->getPosition();
		pos.x = pos.x - this->offsetX + 8;
		pos.y = pos.y - this->offsetY + 10;
		fireworks.push_back(new Firework());
		fireworks[fireworks.size() - 1]->blowUp(pos);
}