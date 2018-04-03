#include "Menu.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Credits.h"



Menu::Menu()
{
}


Menu::~Menu()
{
	

}

void Menu::init() {
	AudioEngine::instance().playMusic("Music/menu.mp3");
	bExit = bPlay = bCredits = bInstructions = false;
	bUp = bDown = pooledUp = pooledDown = false;
	selected = 0;

	initShaders();

	
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);

	bgTexture.loadFromFile("images/backTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);
	buttonTexture.loadFromFile("images/Button_Big.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonTexture.setMinFilter(GL_NEAREST);
	buttonTexture.setMagFilter(GL_NEAREST);
	
	

	title = Sprite::createSprite(glm::vec2(float((446)*0.50f), float((154)*0.50f)), glm::vec2(1.f, 1.f), &titleTexture, &simpleTexProgram);
	title->setPosition(glm::vec2(float((CAMERA_WIDTH / 2) - 446 / 4), 0.0f));

	background = Sprite::createSprite(glm::vec2(1920.f/5.15f, 1080.f/5.15f), glm::vec2(1.f, 1.f),
		&bgTexture, &simpleTexProgram);
	background->setPosition(background->position() + glm::vec2(-20, 0));

	float buttonPosX = (CAMERA_WIDTH / 2) - 420 / 6; // x = 90
	float buttonSizeX = 420 / 3; // x = 140;
	float buttonSizeY = 22; // because 65/3 is not exact...
	playButton			= Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonTexture, &simpleTexProgram);
	playButton->setPosition(glm::vec2(buttonPosX, 80));
	instructionsButton	= Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonTexture, &simpleTexProgram);
	instructionsButton->setPosition(glm::vec2(buttonPosX, 112));
	creditsButton		= Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonTexture, &simpleTexProgram);
	creditsButton->setPosition(glm::vec2(buttonPosX, 144));
	exitButton			= Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonTexture, &simpleTexProgram);
	exitButton->setPosition(glm::vec2(buttonPosX, 176));
	

	// init font
	if (!playText.init("fonts/GILLUBCD.ttf"))
		cout << "Could not load font!!!" << endl;

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void Menu::render() {
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();
	title->render();
	playButton->render();
	instructionsButton->render();
	creditsButton->render();
	exitButton->render();
			
	simpleTexProgram.setUniform4f("color", 1.f, 1.f, 0.f, 1.0f);
	if (selected == 0)
		playText.render("PLAY", glm::vec2(138*3, 97 * 3), 46, colorWhite);
	else playText.render("PLAY", glm::vec2(138 * 3, 97 * 3), 46, colorDarkGreen);
	if (selected == 1)
		playText.render("INSTRUCTIONS", glm::vec2(103 * 3, 129 * 3), 46, colorWhite);
	else playText.render("INSTRUCTIONS", glm::vec2(103 * 3, 129 * 3), 46, colorDarkGreen);
	if (selected == 2)
		playText.render("CREDITS", glm::vec2(125 * 3, 161 * 3), 46, colorWhite);
	else playText.render("CREDITS", glm::vec2(125 * 3, 161 * 3), 46, colorDarkGreen);
	if (selected == 3)
		playText.render("EXIT", glm::vec2(142 * 3, 193 * 3), 46, colorWhite);
	else playText.render("EXIT", glm::vec2(142 * 3, 193 * 3), 46, colorDarkGreen);
			
			
}

void Menu::update(int deltaTime){
	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (!pooledUp) {
			pooledUp = true;
			if (selected != 0) selected--;
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (!pooledDown) {
			pooledDown = true;
			if (selected != 3) selected++;
		}
	}
	if (Game::instance().getKey(13)) {
		if (selected == 3) bExit = true;
		else if (selected == 0) bPlay = true;
		else if (selected == 1) bInstructions = true;
		else if (selected == 2) bCredits = true;
	}
	if (pooledDown && !Game::instance().getSpecialKey(GLUT_KEY_DOWN)) pooledDown = false;
	if (pooledUp && !Game::instance().getSpecialKey(GLUT_KEY_UP)) pooledUp = false;
}

Scene* Menu::changeState() {
	if (bPlay) {
		Scene* scene = new StartScene("maps/Level1.txt");
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().yippee();
		AudioEngine::instance().playMusic("Music/lemmings.wav");
		scene->init();
		return scene;
	}
	else if (bExit) Game::instance().closeGame();
	else if (bCredits) {
		Scene* credits = new Credits();
		AudioEngine::instance().buttonEffect();
		credits->init();
		return credits;
	}
	else if (bInstructions) {
		Scene* instructions = new InstructionsScene();
		AudioEngine::instance().buttonEffect();
		instructions->init();
		return instructions;
	}
	return this;
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





