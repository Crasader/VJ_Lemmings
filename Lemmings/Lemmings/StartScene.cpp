#include "StartScene.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "AudioEngine.h"



StartScene::StartScene(string pathLevel) {
	path = pathLevel;
}


StartScene::~StartScene() {
	if (background != NULL)
		delete background;
	if (okButton != NULL)
		delete okButton;
	if (okSelectedButton != NULL)
		delete okSelectedButton;
	if (menuButton != NULL)
		delete menuButton;
	if (menuSelectedButton != NULL)
		delete menuSelectedButton;
}

void StartScene::init() {
	initShaders();
	currentTime = 0;
	selected = NONE_BUTTON;
	state = ON;

	TextProcessor::instance().loadFileAndProcess(path);
	AudioEngine::instance().playMusic("music/startScreen.mp3");

	bgTexture.loadFromFile("images/backTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);
	background = Sprite::createSprite(glm::vec2(1920.f / 5.15f, 1080.f / 5.15f), glm::vec2(1.f, 1.f),
		&bgTexture, &simpleTexProgram);
	background->setPosition(background->position() + glm::vec2(-20, 0));

	buttonMenuTexture.loadFromFile("images/Button_Short_Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuTexture.setMinFilter(GL_NEAREST);
	buttonMenuTexture.setMagFilter(GL_NEAREST);
	buttonMenuSelectedTexture.loadFromFile("images/Button_Short_Menu_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuSelectedTexture.setMinFilter(GL_NEAREST);
	buttonMenuSelectedTexture.setMagFilter(GL_NEAREST);

	buttonOkTexture.loadFromFile("images/Button_Short_Ok.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonOkTexture.setMinFilter(GL_NEAREST);
	buttonOkTexture.setMagFilter(GL_NEAREST);
	buttonOkSelectedTexture.loadFromFile("images/Button_Short_Ok_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonOkSelectedTexture.setMinFilter(GL_NEAREST);
	buttonOkSelectedTexture.setMagFilter(GL_NEAREST);

	buttonPosY = 175;
	buttonSizeX = 210 / 3; // x = 70;
	buttonSizeY = 22; // because 65/3 is not exact...
	menuButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuTexture, &simpleTexProgram);
	menuButton->setPosition(glm::vec2(20, buttonPosY));
	menuSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuSelectedTexture, &simpleTexProgram);
	menuSelectedButton->setPosition(glm::vec2(20, buttonPosY));
	okButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonOkTexture, &simpleTexProgram);
	okButton->setPosition(glm::vec2(220, buttonPosY));
	okSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonOkSelectedTexture, &simpleTexProgram);
	okSelectedButton->setPosition(glm::vec2(220, buttonPosY));

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/DroidSerif-Bold.ttf")) {
		cout << "Couldn't load font" << endl;
	}
}

void StartScene::update(int deltaTime) {
	currentTime += deltaTime;
	selected = checkButtonsColision();

	if (Game::instance().getLeftMousePressed()) {
		if (selected == MENU_BUTTON) state = MENU_CHOSEN;
		else if (selected == OK_BUTTON) state = OK_CHOSEN;
	}
}

void StartScene::render() {
	glm::mat4 modelview; 

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();

	if (selected == MENU_BUTTON)
		menuSelectedButton->render();
	else
		menuButton->render();

	if (selected == OK_BUTTON)
		okSelectedButton->render();
	else
		okButton->render();
	
	// Level Num
	string numLevel = "Level " + to_string(TextProcessor::instance().levelNumber);
	simpleText.render(numLevel, glm::vec2(20, 80), 64, colorRed);
	// Level Name
	string nameLevel = TextProcessor::instance().levelName;
	simpleText.render(nameLevel, glm::vec2(340, 80), 64, colorRed);
	// Num Lemmings Spawn
	string lemmingsToSpawn = to_string(TextProcessor::instance().lemmings) + " lemmings";
	simpleText.render(lemmingsToSpawn, glm::vec2(300, 200), 48, colorBlue);
	// Num Lemmings Save
	int lemmingsToEnter = (TextProcessor::instance().minLemmings * 100) / TextProcessor::instance().lemmings;
	string lemmingsTobeSaved = to_string(lemmingsToEnter) + "% to be saved";
	simpleText.render(lemmingsTobeSaved, glm::vec2(300, 260), 48, colorGreen);
	// Release Rate
	string releaseRate = "Release rate " + to_string(TextProcessor::instance().spawnrate);
	simpleText.render(releaseRate, glm::vec2(300, 320), 48, colorOrange);
	// Max Time
	string time = "Time " + to_string(TextProcessor::instance().maxTime) + " seconds";
	simpleText.render(time, glm::vec2(300, 380), 48, colorCian);
	
}

Scene * StartScene::changeState() {
	switch (state) {
	case MENU_CHOSEN: {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		menu->init();
		return menu;
	}
	case OK_CHOSEN: {
		Scene* scene = new PlayScene(path);
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}
	}
	return this;

}

StartScene::StartButton StartScene::checkButtonsColision() {
	int mouseX, mouseY;
	Game::instance().getMousePosition(mouseX, mouseY);
	if (mouseY >= buttonPosY * 3 && mouseY <= (buttonPosY + buttonSizeY) * 3) {
		if ((mouseX >= (20 * 3) && mouseX <= ((20 + buttonSizeX) * 3)))
			return MENU_BUTTON;
		else if ((mouseX >= (220 * 3) && mouseX <= ((220 + buttonSizeX) * 3)))
			return OK_BUTTON;
	}
	return NONE_BUTTON;
}

void StartScene::initShaders() {
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

}
