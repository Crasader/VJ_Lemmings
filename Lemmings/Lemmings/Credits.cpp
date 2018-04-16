#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Credits.h"



Credits::Credits() {}


Credits::~Credits() {
	if (background != NULL)
		delete background;
	if (title != NULL)
		delete title;
	if (menuButton != NULL)
		delete menuButton;
	if (menuSelectedButton != NULL)
		delete menuSelectedButton;
}

void Credits::init(){
	initShaders();
	state = ON;
	selected = NONE_BUTTON;
	bgTexture.loadFromFile("images/backTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);
	background = Sprite::createSprite(glm::vec2(1920.f / 5.15f, 1080.f / 5.15f), glm::vec2(1.f, 1.f),
		&bgTexture, &simpleTexProgram);
	background->setPosition(background->position() + glm::vec2(-20, 0));
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);
	title = Sprite::createSprite(glm::vec2(float((446)*0.30f), float((154)*0.30f)), glm::vec2(1.f, 1.f),
		&titleTexture, &simpleTexProgram);
	title->setPosition(glm::vec2(float((CAMERA_WIDTH / 2) - 446 * 0.15), 0.0f));

	buttonMenuTexture.loadFromFile("images/Button_Short_Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuTexture.setMinFilter(GL_NEAREST);
	buttonMenuTexture.setMagFilter(GL_NEAREST);
	buttonMenuSelectedTexture.loadFromFile("images/Button_Short_Menu_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuSelectedTexture.setMinFilter(GL_NEAREST);
	buttonMenuSelectedTexture.setMagFilter(GL_NEAREST);

	buttonSizeX = 210 / 3; // x = 70;
	buttonSizeY = 22; // because 65/3 is not exact...
	menuButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuTexture, &simpleTexProgram);
	menuButton->setPosition(glm::vec2(20, 175));
	menuSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuSelectedTexture, &simpleTexProgram);
	menuSelectedButton->setPosition(glm::vec2(20, 175));

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/upheavtt.ttf")) {
		cout << "Couldn't load font" << endl;
	}
}

void Credits::render(){
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();
	title->render();
	simpleText.render("Replica of the", glm::vec2(446 * 0.15, textRPos ), 32, colorWhite);
	simpleText.render("Lemmings DOS game made by:", glm::vec2(446 * 0.15, textRPos+32), 32, colorWhite);
	simpleText.render("Isabel Codina Garcia", glm::vec2(446 * 0.15, textRPos+96), 32, colorGreen);
	simpleText.render("Borja Fernandez Ruizdelgado", glm::vec2(446 * 0.15, textRPos + 128), 32, colorGreen);



	simpleTexProgram.use();
	if (selected == MENU_BUTTON)
		menuSelectedButton->render();
	else
		menuButton->render();

}

void Credits::update(int deltaTime){
	selected = checkButtonsColision();

	if (Game::instance().getLeftMousePressed()) 
		if (selected == MENU_BUTTON) state = MENU_CHOSEN;
	
}

Scene * Credits::changeState() {
	switch (state) {
	case MENU_CHOSEN: {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		menu->init();
		return menu;
	}
	}
	return this;
}

Credits::CreditsButton Credits::checkButtonsColision() {
	int mouseX, mouseY;
	Game::instance().getMousePosition(mouseX, mouseY);
	if (mouseY >= 175 * 3 && mouseY <= (175 + buttonSizeY) * 3) {
		if ((mouseX >= (20 * 3) && mouseX <= ((20 + buttonSizeX) * 3)))
			return MENU_BUTTON;
		
	}
	return NONE_BUTTON;
}

void Credits::initShaders(){
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

