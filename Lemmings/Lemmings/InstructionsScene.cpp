#include "InstructionsScene.h"




InstructionsScene::InstructionsScene(){
}


InstructionsScene::~InstructionsScene(){
}

void InstructionsScene::init(){
	initShaders();
	exitToMenu = false;
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
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if (!simpleText.init("fonts/Cartoon_Regular.ttf")) {
		cout << "Couldn't load font" << endl;
	}
	
}

void InstructionsScene::render(){
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();
	title->render();
	simpleText.render("descibir funcionalidades del juego aqui:", glm::vec2(446 * 0.15, textRPos), 32, colorWhite);
}

void InstructionsScene::update(int deltaTime){
	if (Game::instance().getKey(27)) exitToMenu = true;
}

Scene * InstructionsScene::changeState(){
	if (exitToMenu) {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		menu->init();
		return menu;
	}
	else return this;
}

void InstructionsScene::initShaders(){
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
