#include "PlayScene.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Cursor.h"



PlayScene::PlayScene(string levelPath) {
	map = NULL;
	path = levelPath;
}

PlayScene::~PlayScene() {
	if (map != NULL) {
		delete map;
	}
}

void PlayScene::init() {
	bExit = bEnd = bMouseLeft = bMoveCameraRight = bMoveCameraLeft = false;
	bDigger = bBasher = bBlocker = bClimber = bBuilder = bFloater = bBomber = bExplosion = false;
	
	initShaders();

	cameraX = TextProcessor::instance().camPos.x;
	cameraY = TextProcessor::instance().camPos.y;

	numDiggers = TextProcessor::instance().numbDig;
	numBlockers = TextProcessor::instance().numbStop;
	numBashers = TextProcessor::instance().numbBash;
	numFloaters = TextProcessor::instance().numbFlo;
	numClimbers = TextProcessor::instance().numbCli;
	numBombers = TextProcessor::instance().numbBomb;
	numBuilders = TextProcessor::instance().numbBuild;
	
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(TextProcessor::instance().width), float(TextProcessor::instance().height)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	
	/*
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f) };
	*/

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(TextProcessor::instance().path, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(TextProcessor::instance().mPath, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(cameraX, cameraX + float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	manager = new EntityManager(TextProcessor::instance().lemmings, TextProcessor::instance().startDoor, TextProcessor::instance().doorStartColor, TextProcessor::instance().endDoor,
		TextProcessor::instance().doorEndColor, simpleTexProgram, &colorTexture, &maskTexture, "images/start_spritesheet.png", "images/end_spritesheet.png");
	gui = new InterfazUsuario();
	setGUI();
	gui->init(colorTexture, maskTexture, cameraX, cameraY);
	
}

void PlayScene::update(int deltaTime) {
	currentTime += deltaTime;
	int buttonAnt = buttonPressed;
	buttonPressed = gui->getButtonPressed();
	if (buttonPressed != 9 && buttonAnt == 9) decreaseSceneSpeed();
	else if (buttonPressed != 10 && buttonAnt == 10) manager->resetNormalSpeed();
	manager->update(deltaTime, buttonPressed);
	
	
	
	if (Game::instance().getKey(27)) bExit = true;
	/*
	if (Game::instance().getKey('1')) bDigger = true;
	if (Game::instance().getKey('2')) bBlocker = true;
	if (Game::instance().getKey('3')) bBasher = true;
	if (Game::instance().getKey('4')) bClimber = true;
	if (Game::instance().getKey('5')) bBuilder = true;
	if (Game::instance().getKey('6')) bFloater = true;
	if (Game::instance().getKey('7')) bBomber = true;
	if (Game::instance().getKey('8')) bExplosion = true;
	*/



	int x = 0, y = 0;
	Game::instance().getMousePosition(x, y);
	if (manager->lemmingInCursor(cameraX + x/3, cameraY + y/3)) {
		Cursor::instance().lemmingInside(true);
	}
	else {
		Cursor::instance().lemmingInside(false);
	}
	if (x > 900 && y < 495) bMoveCameraRight = true;
	if (x < 60 && y < 495) bMoveCameraLeft = true;

	if (Game::instance().getLeftMousePressed()) bMouseLeft = true;
	gui->setLemmingsIn(manager->getLemmingsExited());
	gui->update(x/3,  y/3);


	int lemmingsStillAlive = TextProcessor::instance().lemmings - manager->getLemmingsDied() - manager->getLemmingsExited();
	if (lemmingsStillAlive == 0) bEnd = true;

}

void PlayScene::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	manager->render();
	
	gui->render();
	
}

Scene * PlayScene::changeState() {
	if (bEnd) {
		Scene* scene = new EndScene(path, manager->getLemmingsExited());
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		scene->init();
		return scene;
	}
	if (bExit) {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		menu->init();
		return menu;
	}
	if (bDigger) {
		manager->changeLemmingState(1);
		bDigger = false;
	}
	else if (bBlocker) {
		manager->changeLemmingState(2);
		bBlocker = false;
	}
	else if (bBasher) {
		manager->changeLemmingState(3);
		bBasher = false;
	}
	else if (bClimber) {
		manager->changeLemmingState(4);
		bClimber = false;
	}
	else if (bBuilder) {
		manager->changeLemmingState(5);
		bBuilder = false;
	}
	else if (bFloater) {
		manager->changeLemmingState(6);
		bFloater = false;
	}
	else if (bBomber) {
		manager->changeLemmingState(7);
		bBomber = false;
	}
	else if (bExplosion) {
		manager->changeLemmingState(8);
		bExplosion = false;
	}
	if (bMouseLeft) {
		int buttonPressed1 = gui->getButtonPressed();
		int x = 0, y = 0;
		int effect = -1;
		if (buttonPressed == 0) effect = 3;
		else if (buttonPressed == 1) effect = 2;
		else if (buttonPressed == 2) effect = 4;
		else if (buttonPressed == 3) effect = 1;
		else if (buttonPressed == 4) effect = 6;
		else if (buttonPressed == 5) effect = 7;
		else if (buttonPressed == 6) effect = 5;
		else if (buttonPressed1 == 7) {
			manager->increaseSpawnTime();
			gui->decreaseSpawnRate();
		}
		else if (buttonPressed1 == 8) {
			gui->increaseSpawnRate();
			manager->decreaseSpawnTime();
		}
		else if (buttonPressed == 9) doubleSceneSpeed();
		else if (buttonPressed == 10) manager->pause();
		else if (buttonPressed == 11) {
			manager->resetNormalSpeed();
			manager->killAllLemmings();
			
		}
		if (effect != -1) {
			effectForLemming(x, y, effect);
			setGUI();
		}

		
		bMouseLeft = false;
	}
	
	if (bMoveCameraRight || bMoveCameraLeft) {
		if (bMoveCameraRight && cameraX < (TextProcessor::instance().width - CAMERA_WIDTH)) cameraX += 2;
		else if (bMoveCameraLeft && cameraX > 0) cameraX -= 2;
		bMoveCameraRight = false;
		bMoveCameraLeft = false;
	}
	projection = glm::ortho(0.f + cameraX, float(CAMERA_WIDTH - 1) + cameraX, float(CAMERA_HEIGHT - 1), 0.f);
	
	return this;
}

void PlayScene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + int(cameraX);
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 0);
}

void PlayScene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + int(cameraX);
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
}

void PlayScene::effectForLemming(int mouseX, int mouseY, int effect) {
	int x, y;
	Game::instance().getMousePosition(x, y);
	bool lemmingChanged = false;
	if (effect == 1 && numDiggers > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 2 && numBlockers > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 3 && numBashers > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 4 && numClimbers > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 5 && numBuilders > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 6 && numFloaters > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
	else if (effect == 7 && numBombers > 0) lemmingChanged = manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);

	 
	if (lemmingChanged) {
		if (effect == 1) numDiggers--;
		else if (effect == 2) numBlockers--;
		else if (effect == 3) numBashers--;
		else if (effect == 4) numClimbers--;
		else if (effect == 5) numBuilders--;
		else if (effect == 6) numFloaters--;
		else if (effect == 7) numBombers--;
	}
}

void PlayScene::setGUI() {
	gui->setBashers(numBashers);
	gui->setDiggers(numDiggers);
	gui->setClimbers(numClimbers);
	gui->setBlockers(numBlockers);
	gui->setBuilders(numBuilders);
	gui->setFloaters(numFloaters);
	gui->setBombers(numBombers);

	
}

void PlayScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void PlayScene::doubleSceneSpeed() {
	manager->doubleSpeedAnimation();
}

void PlayScene::decreaseSceneSpeed() {
	manager->resetNormalSpeed();
}

