#include "GUI.h"



GUI::GUI()
{
}


GUI::~GUI()
{
}

void GUI::init()
{
	out = 0;
	in = 0;
	initShader();
	if(info.init("fonts/Cartoon_Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void GUI::render()
{
}

void GUI::update(int deltaTime)
{
}

Scene * GUI::changeState()
{
	return nullptr;
}

void GUI::setTime(int time)
{
	this->time = time;
}

void GUI::setDiggers(int digger)
{
	this->digger = digger;
}

void GUI::setClimbers(int climber)
{
	this->climber = climber;
}

void GUI::setSpawnRate(int spawnrate)
{
	this->spawnRate = spawnRate;
}

void GUI::initShader()
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
