#include "InterfazUsuario.h"
#include <thread>         
#include <chrono> 



InterfazUsuario::InterfazUsuario()
{
}


InterfazUsuario::~InterfazUsuario()
{
}

void InterfazUsuario::init(Texture & colorTexture, VariableTexture & maskTexture, float & camX, float & camY)
{	
	initShader();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(TextProcessor::instance().width), float(TextProcessor::instance().height)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	/*
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f) };
	*/

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	this->colorTexture = colorTexture;
	this->maskTexture = maskTexture;
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
	this->camX = &camX;
	this->camY = &camY;
	if (!info.init("fonts/upheavtt.ttf"))
		cout << "Could not load font!!!" << endl;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	placeButtons();
	frame.loadFromFile("images/marcoMinimapa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	frame.setMinFilter(GL_NEAREST);
	frame.setMagFilter(GL_NEAREST);
	marco = Sprite::createSprite(glm::ivec2(64,32), glm::vec2(1, 1), &frame, &simpleTexProgram);
	marco->setPosition(glm::vec2(CAMERA_WIDTH - 64, CAMERA_HEIGHT - 32));
	scaleFactor = ((float)64 / (float)TextProcessor::instance().width);
	selector.loadFromFile("images/selectorMinimapa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selector.setMinFilter(GL_NEAREST);
	selector.setMagFilter(GL_NEAREST);
	camSelector = Sprite::createSprite(glm::ivec2(320 * scaleFactor, 32), glm::vec2(1, 1), &selector, &simpleTexProgram);
	
}

void InterfazUsuario::render()
{	
		glm::mat4 modelview;
	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(CAMERA_WIDTH - 64, CAMERA_HEIGHT- (48 / 1.5) + 6, 0));
	modelview = glm::scale(modelview, glm::vec3(scaleFactor, scaleFactor, 1));
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);
	
	modelview = glm::mat4(1.0f);
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	
	marco->render();
	camSelector->setPosition(glm::vec2(CAMERA_WIDTH - (TextProcessor::instance().width - *camX)*scaleFactor, CAMERA_HEIGHT - 32));
	camSelector->render();
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

		if (checkColisionMinimap(mouseX, mouseY)) {
			float topMiniMap = CAMERA_WIDTH - 64;
			float diff = mouseX - topMiniMap;
			*camX = diff / scaleFactor;
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

bool InterfazUsuario::checkColisionMinimap(int mouseX, int mouseY)
{
	float topMiniMapX = CAMERA_WIDTH - 64;
	float topMiniMapY = CAMERA_HEIGHT - 32;
	if (mouseX >= topMiniMapX && mouseX <= CAMERA_WIDTH && mouseY >= topMiniMapY && mouseY <= CAMERA_HEIGHT) return true;
	else return false;
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
