#include "StartScene.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>



StartScene::StartScene() {}


StartScene::~StartScene() {}

void StartScene::init() {
	initShaders();

	bgTexture.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f), &bgTexture, &simpleTexProgram);
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/Cartoon_Regular.ttf")) {
		cout << "Couldn't load font" << endl;
	}
}

void StartScene::update(int deltaTime) {
	currentTime += deltaTime;
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
	simpleText.render("Level x", glm::vec2(20, 20), 32, colorRed);
	// Level Name
	simpleText.render("Level name", glm::vec2(60, 20), 32, colorRed);
	// Num Lemmings Spawn
	simpleText.render("Num Lemmings Spawn", glm::vec2(80, 40), 24, colorGreen);
	// Num Lemmings Save
	simpleText.render("Num Lemmings Sav", glm::vec2(80, 64), 24, colorCian);
	// Max Time
	simpleText.render("Max Time", glm::vec2(80, 88), 24, colorMagenta);
	// Click to continue
	simpleText.render("Click to continue", glm::vec2(20, 110), 32, colorYellow);

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
