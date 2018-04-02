#include "EndScene.h"



EndScene::EndScene(string levelPath, int lemmingsExited) {
	path = levelPath;
	numLemmingsExited = lemmingsExited;
	numLemmingsMin = numLemmingsTotal = 0;
}


EndScene::~EndScene() {}

void EndScene::init() {
	bExit = bContinue = bRetry = false;
	numLemmingsMin = TextProcessor::instance().minLemmings;
	numLemmingsTotal = TextProcessor::instance().lemmings;
	win = (numLemmingsExited >= numLemmingsMin);
	initShaders();

	bgTexture.loadFromFile("images/rockTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(470.f, 464.f), glm::vec2(10.f, 10.f), &bgTexture, &simpleTexProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	if (!simpleText.init("fonts/DroidSerif-Bold.ttf")) {
		cout << "Couldn't load font" << endl;
	}

	int levelNum = TextProcessor::instance().levelNumber;
	if (levelNum == 1) newPath = "maps/Level2.txt";
	else if (levelNum == 2) newPath = "maps/Level3.txt";
	else if (levelNum == 3) newPath = "maps/Level4.txt";
}

void EndScene::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getKey(27)) bExit = true;
	if (Game::instance().getKey(13)) bContinue = true;
	if (Game::instance().getKey('r')) bRetry = true;
}




Scene * EndScene::changeState() {
	if (bExit) {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		menu->init();
		return menu;
	}
	else if (bContinue && win) {
		Scene* scene = new StartScene(newPath);
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}
	else if (bRetry) {
		Scene* scene = new PlayScene(path);
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}

	return this;
}

void EndScene::render() {
	glm::mat4 modelview;

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();

	// Saved
	string saved = "Saved: " + to_string(numLemmingsExited);
	simpleText.render(saved, glm::vec2(340, 200), 48, colorOrange);

	// Required
	string required = "Required: " + to_string(numLemmingsMin);
	simpleText.render(required, glm::vec2(340, 260), 48, colorOrange);

	// Message
	string message = "";
	if (numLemmingsExited == numLemmingsTotal) message = "You rescued every lemming on that level";
	else if (numLemmingsExited >= numLemmingsMin) message = "You rescued enough lemmings";
	else if (numLemmingsExited == 0) message = "Oh dear. Not even one poor Lemming saved";
	else message = "You didn't rescue enough lemmings";
	simpleText.render(message, glm::vec2(30, 320), 38, colorOrange);

	// Continue
	string next = "";
	if (win) next = "Press Enter to next level. Escape to menu. r to Retry.";
	else next = "Press Escape to menu. r to Retry.";
	simpleText.render(next, glm::vec2(20, 580), 34, colorYellow);

}

void EndScene::initShaders() {
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
