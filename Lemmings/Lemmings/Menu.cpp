#include "Menu.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>



Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::init() {
	bExit = false;
	bPlay = false;
	bCredits = false;
	bInstructions = false;
	bUp = false;
	bDown = false;
	colorGreen = glm::vec4(0, 1, 0, 1);
	colorWhite = glm::vec4(1, 1, 1, 1);
	selected = 0;

	initShaders();

	//inicializar texturas y sprites
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);
	bgTexture.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);

	title = Sprite::createSprite(glm::vec2(float((446)*0.30f), float((154)*0.30f)), glm::vec2(1.f, 1.f),
		&titleTexture, &simpleTexProgram);
	title->setPosition(glm::vec2(float((CAMERA_WIDTH / 2) - 446 * 0.15), 0.0f));

	backgrownd = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f),
		&bgTexture, &simpleTexProgram);

	//inicializar texto
	if (!playText.init("fonts/Cartoon_Regular.ttf") || !exitText.init("fonts/Cartoon_Regular.ttf") 
		|| !instructionsText.init("fonts/Cartoon_Regular.ttf") || !creditsText.init("fonts/Cartoon_Regular.ttf"))
		cout << "Could not load font!!!" << endl;

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void Menu::initShaders() {
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

void Menu::render() {
			glm::mat4 modelview;
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			backgrownd->render();
			title->render();

			simpleTexProgram.setUniform4f("color", 1.f, 1.f, 0.f, 1.0f);
		
			if (selected == 3)
				exitText.render("EXIT", glm::vec2((CAMERA_WIDTH * 3 / 2) - 446 * 0.15, 64.f), 160, colorGreen);
			else exitText.render("EXIT", glm::vec2((CAMERA_WIDTH * 3 / 2) - 446 * 0.15, 64.f), 160, colorWhite);
			
			if(selected == 0)
				playText.render("PLAY", glm::vec2((CAMERA_WIDTH * 3 / 2) - 446 * 0.15, 64.f), 160-64, colorGreen);
			else playText.render("PLAY", glm::vec2((CAMERA_WIDTH * 3 / 2) - 446 * 0.15, 64.f), 160-64, colorWhite);
}

void Menu::update(int deltaTime){
	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (selected != 0) selected--;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (selected != 3) selected++;
	}
	if (Game::instance().getKey('y')) {
		if (selected == 3) bExit = true;
		else if (selected == 1) bPlay = true;
		else if (selected == 2) bInstructions = true;
		else if (selected == 3) bCredits = true;
	}
}

Scene* Menu::changeState() {
	if (bPlay) {}
	else if (bExit) Game::instance().closeGame();
	return this;
}





