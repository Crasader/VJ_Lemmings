#include "Button.h"
#include <iostream>
#include "Game.h"



Button::Button(glm::ivec2 size, glm::vec2 position, string texture,string number)
{
	pressed = false;
	this->size = size;
	this->number = number;
	this->position = position;
	image.loadFromFile(texture, TEXTURE_PIXEL_FORMAT_RGBA);
	image.setMinFilter(GL_NEAREST);
	image.setMagFilter(GL_NEAREST);
	selected.loadFromFile("images/GUI/Button_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selected.setMinFilter(GL_NEAREST);
	selected.setMagFilter(GL_NEAREST);
	initShader();
	button = Sprite::createSprite(size, glm::vec2(1, 1), &image, &shaderProgram);
	button->setPosition(position);
	selectedButton = Sprite::createSprite(size, glm::vec2(1, 1), &selected, &shaderProgram);
	selectedButton->setPosition(position);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!buttonText.init("fonts/Cartoon_Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	
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
	if (pressed) selectedButton->render();
	buttonText.render(number, glm::vec2(position.x*3 + 15, position.y*3 + 30), 20, colorRed);
	
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

void Button::update(string number, bool pressed) {
	//this->number = number;
	//this->pressed = pressed;
}

bool Button::checkColision(int mouseX, int mouseY)
{	
	
	if (mouseX >= position.x && mouseX <= position.x + size.x && mouseY >= position.y && mouseY <= position.y + size.y) {
		return true;
	}
	else return false;
}

void Button::select()
{
	pressed = true;
}

void Button::deselect()
{
	pressed = false;
}

