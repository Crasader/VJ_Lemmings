#include "InterfazUsuario.h"
#include <thread>         
#include <chrono> 



InterfazUsuario::InterfazUsuario()
{
}


InterfazUsuario::~InterfazUsuario()
{
}

void InterfazUsuario::init()
{
	out = 0;
	in = 0;
	buttonSelected = -1;
	spawnRate = 0;
	climber = 0;
	time = 0;
	blocker = 0;
	basher = 0;
	digger = 0;
	floater = 0;
	bomber = 0;
	builder = 0;

	initShader();
	if (!info.init("fonts/upheavtt.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	placeButtons();
}

void InterfazUsuario::render()
{
	renderButtons();
	info.render("  OUT: " + to_string(out) + "  IN: " + to_string(in) + "  TIME: " + to_string(time), glm::vec2((CAMERA_WIDTH/ 2), CAMERA_HEIGHT*3 - 35*3), 35, colorGreen);
}

void InterfazUsuario::update(int mouseX, int mouseY)
{
	if (Game::instance().getLeftMousePressed()) {
		for (int i = 0; i < (int)buttons.size(); ++i) {
			if (buttons[i]->checkColision(mouseX, mouseY)) {
				if (buttonSelected != i || buttonSelected == 7 || buttonSelected == 8) {
					if (buttonSelected >= 0) buttons[buttonSelected]->deselect();
					buttonSelected = i;
					if (buttonSelected != 7 && buttonSelected != 8) {
						buttons[buttonSelected]->select();
						AudioEngine::instance().buttonEffect();
					}
					
				}
				break;
			}
		}
	}
}

void InterfazUsuario::setTime(int time)
{
	this->time = time;
}

void InterfazUsuario::setDiggers(int digger)
{
	this->digger = digger;
}

void InterfazUsuario::setClimbers(int climber)
{
	this->climber = climber;
}

void InterfazUsuario::increaseSpawnRate()
{
	this->spawnRate++;
	buttons[8]->increaseText();
	buttons[7]->decreaseText();
}

void InterfazUsuario::decreaseSpawnRate() {
	this->spawnRate--;
	buttons[7]->increaseText();
	buttons[8]->decreaseText();
}

void InterfazUsuario::setBlockers(int bloker) {
	this->blocker = bloker;
}

void InterfazUsuario::setBashers(int basher) {
	this->basher = basher;
}

void InterfazUsuario::initShader()
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

void InterfazUsuario::placeButtons()
{
	int i = 0;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Basher.png", to_string(basher)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Blocker.png", to_string(blocker)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Climber.png", to_string(climber)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Digger.png", to_string(digger)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Floater.png", to_string(floater))); 
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Bomber.png", to_string(bomber)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Builder.png", to_string(builder)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Decrease_Release_Rate.png", to_string(50 - spawnRate)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Increase_Release_Rate.png", to_string(50 + spawnRate)));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Speed.png", ""));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Pause.png", ""));
	++i;
	buttons.push_back(new Button(glm::ivec2(32 / 1.5, 48 / 1.5), glm::vec2(i*(32 / 1.5), CAMERA_HEIGHT - 48 / 1.5), "images/GUI/Button_Armageddon.png", ""));

}

void InterfazUsuario::renderButtons()
{

	for (int i = 0; i < (int)buttons.size(); ++i) {
		buttons[i]->render();
	}
}

void InterfazUsuario::setLemmingsOut(int out) {
	this->out = out;
}

void InterfazUsuario::setLemmingsIn(int in) {
	this->in = in;
}

void InterfazUsuario::setBomber(int bomber)
{
	this->bomber = bomber;
}

int InterfazUsuario::getButtonPressed() {
	return buttonSelected;
}
