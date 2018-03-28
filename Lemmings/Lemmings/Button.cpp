#include "Button.h"
#include <iostream>



Button::Button(glm::ivec2 size, glm::vec2 position, string texture)
{
	image.loadFromFile(texture, TEXTURE_PIXEL_FORMAT_RGBA);
	image.setMinFilter(GL_NEAREST);
	image.setMagFilter(GL_NEAREST);
	initShader();
	button = Sprite::createSprite(size, glm::vec2(1, 1), &image, &shaderProgram);
	button->setPosition(position);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	
}

Button::~Button()
{
}

void Button::render() {
	glm::mat4 modelview;
	shaderProgram.use();
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	shaderProgram.setUniformMatrix4f("modelview", modelview);
	button->render();
}

void Button::initShader()
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
	shaderProgram.init();
	shaderProgram.addShader(vShader);
	shaderProgram.addShader(fShader);
	shaderProgram.link();
	if (!shaderProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << shaderProgram.log() << endl << endl;
	}
	shaderProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

}
