#include "Menu.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

void Menu::render(){
	if (!scene) {
		glm::mat4 modelview;
		simpleTexProgram.use();
		simpleTexProgram.setUniformMatrix4f("projection", projection);
		simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		simpleTexProgram.setUniformMatrix4f("modelview", modelview);
		fondo->render();
		titulo->render();

		simpleTexProgram.setUniform4f("color", 1.f, 1.f, 0.f, 1.0f);



		if (changeDirection) {
			changeDirection = false;
			if (play) {

				color1 = glm::vec4(1, 1, 1, 1);
				color2 = glm::vec4(0, 1, 0, 1);
				play = false;
				exit = true;

			}
			else {
				color1 = glm::vec4(0, 1, 0, 1);
				color2 = glm::vec4(1, 1, 1, 1);
				play = true;
				exit = false;

			}
		}
		playText.render("PLAY", glm::vec2(CAMERA_WIDTH * 3 / 2 - 446 * 0.15, CAMERA_HEIGHT * 3 / 2), 46, color1);
		exitText.render("EXIT", glm::vec2(CAMERA_WIDTH * 3 / 2 - 446 * 0.15, CAMERA_HEIGHT * 3 / 2 + 64), 46, color2);
	}
	else {
		sceneObj.render();
	}

	
}

void Menu::init(){
	exit = false;
	play = false;
	scene = false;
	color1, color2 = glm::vec4(1, 1, 1, 1);
	color1 = color2;
	changeDirection = false;
	initShaders();
	texturaTitulo.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texturaTitulo.setMinFilter(GL_NEAREST);
	texturaTitulo.setMagFilter(GL_NEAREST);
	texturaFondo.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);

	
	
	titulo = Sprite::createSprite(glm::vec2(float((446)*0.30f), float((154)*0.30f)), glm::vec2(1.f, 1.f),
		&texturaTitulo,&simpleTexProgram);
	titulo->setPosition(glm::vec2(float((CAMERA_WIDTH/2)-446*0.15), 0.0f));


	fondo = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f),
		&texturaFondo, &simpleTexProgram);



	if (!playText.init("fonts/Cartoon_Regular.ttf") || !exitText.init("fonts/Cartoon_Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	
	

	

}

void Menu::selection() {
	this->changeDirection = true;
}

void Menu::initShaders(){
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

void Menu::enter() {
	if (play) {
		if (!scene) {
			scene = true;
			sceneObj.init();
		}
		
	}
	if (exit) {
		Game::instance().closeGame();
	}
}

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	if (scene) {
		sceneObj.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
}

void Menu::update(int deltaTime) {
	if (scene) {
		sceneObj.update(deltaTime);
	}
}

void Menu::keyPressed(int key) {
	if(key == 'y')
		enter();
}

void Menu::specialKey(int key) {
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
		selection();
	}
}



