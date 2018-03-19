#include "Credits.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>



Credits::Credits()
{
}


Credits::~Credits()
{
	
}

void Credits::init(){
	initShaders();
	exitToMenu = false;
	bgTexture.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f),
		&bgTexture, &simpleTexProgram);
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);
	title = Sprite::createSprite(glm::vec2(float((446)*0.30f), float((154)*0.30f)), glm::vec2(1.f, 1.f),
		&titleTexture, &simpleTexProgram);
	title->setPosition(glm::vec2(float((CAMERA_WIDTH / 2) - 446 * 0.15), 0.0f));
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/Cartoon_Regular.ttf")) {
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
	float textRPos = (CAMERA_HEIGHT * 3 / 2) - 32;
	simpleText.render("Replica of the Lemmings DOS game made by:", glm::vec2(446 * 0.15, textRPos ), 32, colorWhite);
	simpleText.render("Isabel Codina Garcia", glm::vec2(446 * 0.15, textRPos+64), 32, colorGreen);
	simpleText.render("Borja Fernandez Ruizdelgado", glm::vec2(446 * 0.15, textRPos + 96), 32, colorGreen);

	simpleText.render("Press ESC to return to the main menu", glm::vec2(CAMERA_WIDTH / 2, CAMERA_HEIGHT * 3 - 50), 32 ,colorWhite);

}

void Credits::update(int deltaTime){
	if (Game::instance().getKey(27)) exitToMenu = true;
}

Scene * Credits::changeState()
{
	if (exitToMenu) {
		Scene* menu = new Menu();
		menu->init();
		return menu;
	}
	else return this;
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

