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
	buttonSelected = -1;
	initShader();
	if(info.init("fonts/Cartoon_Regular.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	placeButtons();
}

void GUI::render()
{
	renderButtons();
}

void GUI::update(int mouseX,int mouseY)
{
	if (Game::instance().getLeftMousePressed()) {
		for (int i = 0; i < (int)buttons.size(); ++i) {
			if (buttons[i]->checkColision(mouseX,mouseY)) {
				if (buttonSelected != i) {
					if(buttonSelected >= 0) buttons[buttonSelected]->deselect();
					buttonSelected = i;
					buttons[buttonSelected]->select();
				}
				break;
			}
		}
	}
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

void GUI::placeButtons()
{
	int i = 0;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Basher.png", "5"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Blocker.png", "5"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Climber.png", "5"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Digger.png", "5"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Decrease_Release_Rate.png", "50"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Increase_Release_Rate.png", "50"));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Speed.png", ""));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Pause.png", ""));

}

void GUI::renderButtons()
{

	for (int i = 0; i < (int)buttons.size(); ++i) {
		buttons[i]->render();
	}
}
