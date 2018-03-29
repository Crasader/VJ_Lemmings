#include "PlayScene.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Cursor.h"



PlayScene::PlayScene() {
	map = NULL;
}

PlayScene::~PlayScene()
{
	if (map != NULL) {
		delete map;
	}
}

void PlayScene::init()
{
	bExit = bMouseLeft = bMouseRight = bMoveCameraRight = bMoveCameraLeft = false;
	bDigger = bBasher = bBlocker = bClimber = false;
	cameraX = 120;
	cameraY = 0;
	initShaders();

	
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(512.f, 256.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	
	/*
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f) };
	*/

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/maps/fun4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/maps/fun4_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(cameraX, cameraX + float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	manager = new EntityManager(2, glm::vec2(180, 30), simpleTexProgram,&maskTexture);
	gui = new InterfazUsuario();
	gui->init();


	spritesheetStart.loadFromFile("images/start_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorStart = new DoorStart(DoorStart::BROWN);
	doorStart->init(glm::vec2(60, 10), simpleTexProgram, spritesheetStart);

	spritesheetStart.loadFromFile("images/end_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetStart.setMinFilter(GL_NEAREST);
	spritesheetStart.setMagFilter(GL_NEAREST);
	doorEnd = new DoorEnd(DoorEnd::BLACK);
	doorEnd->init(glm::vec2(0, 0), simpleTexProgram, spritesheetEnd);
}

void PlayScene::update(int deltaTime)
{
	currentTime += deltaTime;
	
	manager->update(deltaTime);
	


	if (Game::instance().getKey(27)) bExit = true;
	if (Game::instance().getKey('1')) bDigger = true;
	if (Game::instance().getKey('2')) bBlocker = true;
	if (Game::instance().getKey('3')) bBasher = true;
	if (Game::instance().getKey('4')) bClimber = true;


	int x = 0, y = 0;
	Game::instance().getMousePosition(x, y);
	if (manager->lemmingInCursor(cameraX + x/3, cameraY + y/3)) {
		Cursor::instance().lemmingInside(true);
	}
	else {
		Cursor::instance().lemmingInside(false);
	}
	if (x > 900) bMoveCameraRight = true;
	if (x < 60) bMoveCameraLeft = true;

	if (Game::instance().getLeftMousePressed()) bMouseLeft = true;
	if (Game::instance().getRightMousePressed()) bMouseRight = true;
	gui->update( x/3,  y/3);

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
	doorStart->render();
	doorEnd->render();
}

Scene * PlayScene::changeState()
{
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
	if (bMouseLeft) {
		int x = 0, y = 0;
		Game::instance().getMousePosition(x, y);
		//eraseMask(x, y);
		effectForLemming(x, y, 1);
		bMouseLeft = false;
	}
	if (bMouseRight) {
		int x = 0, y = 0;
		Game::instance().getMousePosition(x, y);
		//applyMask(x, y);
		
		bMouseRight = false;
	}
	if (bMoveCameraRight || bMoveCameraLeft) {
		if (bMoveCameraRight) cameraX += 2;
		else if (bMoveCameraLeft) cameraX -= 2;
		projection = glm::ortho(0.f+cameraX, float(CAMERA_WIDTH - 1)+cameraX, float(CAMERA_HEIGHT - 1), 0.f);
		bMoveCameraRight = false;
		bMoveCameraLeft = false;

	}
	
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

void PlayScene::effectForLemming(int mouseX, int mouseY, int effect)
{
	int x, y;
	Game::instance().getMousePosition(x, y);
	manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect);
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



