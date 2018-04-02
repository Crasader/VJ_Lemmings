#include "StartScene.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>



StartScene::StartScene(string pathLevel) {
	path = pathLevel;
}


StartScene::~StartScene() {}

void StartScene::init() {
	bExit = bContinue = false;
	initShaders();

	TextProcessor::instance().loadFileAndProcess(path);

	bgTexture.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f), &bgTexture, &simpleTexProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/DroidSerif-Bold.ttf")) {
		cout << "Couldn't load font" << endl;
	}
}

void StartScene::update(int deltaTime) {
	currentTime += deltaTime;

	if (Game::instance().getKey(27)) bExit = true;
	if (Game::instance().getKey(13)) bContinue = true;
}

void StartScene::render() {
	glm::mat4 modelview;

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();

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
	// Click to continue
	simpleText.render("Press Enter to continue or Escape to go back to menu", glm::vec2(20, 580), 34, colorYellow);

}

Scene * StartScene::changeState() {
	if (bExit) {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		menu->init();
		return menu;
	}
	else if (bContinue) {
		Scene* scene = new PlayScene();
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}

	return this;

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
