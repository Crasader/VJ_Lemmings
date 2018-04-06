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
	state = ON;
	initShaders();
	currentTime = 0;

	bgTexture.loadFromFile("images/backTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);
	background = Sprite::createSprite(glm::vec2(1920.f / 5.15f, 1080.f / 5.15f), glm::vec2(1.f, 1.f),
		&bgTexture, &simpleTexProgram);

	buttonMenuTexture.loadFromFile("images/Button_Short_Menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuTexture.setMinFilter(GL_NEAREST);
	buttonMenuTexture.setMagFilter(GL_NEAREST);
	buttonMenuSelectedTexture.loadFromFile("images/Button_Short_Menu_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonMenuSelectedTexture.setMinFilter(GL_NEAREST);
	buttonMenuSelectedTexture.setMagFilter(GL_NEAREST);

	buttonNextTexture.loadFromFile("images/Button_Short_Next.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonNextTexture.setMinFilter(GL_NEAREST);
	buttonNextTexture.setMagFilter(GL_NEAREST);
	buttonNextSelectedTexture.loadFromFile("images/Button_Short_Next_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonNextSelectedTexture.setMinFilter(GL_NEAREST);
	buttonNextSelectedTexture.setMagFilter(GL_NEAREST);

	buttonRetryTexture.loadFromFile("images/Button_Short_Retry.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonRetryTexture.setMinFilter(GL_NEAREST);
	buttonRetryTexture.setMagFilter(GL_NEAREST);
	buttonRetrySelectedTexture.loadFromFile("images/Button_Short_Retry_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonRetrySelectedTexture.setMinFilter(GL_NEAREST);
	buttonRetrySelectedTexture.setMagFilter(GL_NEAREST);

	buttonPosY = 175;
	buttonSizeX = 210 / 3; // x = 70;
	buttonSizeY = 22; // because 65/3 is not exact...
	menuButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuTexture, &simpleTexProgram);
	menuButton->setPosition(glm::vec2(20, buttonPosY));
	menuSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonMenuSelectedTexture, &simpleTexProgram);
	menuSelectedButton->setPosition(glm::vec2(20, buttonPosY));
	retryButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonRetryTexture, &simpleTexProgram);
	retryButton->setPosition(glm::vec2(120, buttonPosY));
	retrySelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonRetrySelectedTexture, &simpleTexProgram);
	retrySelectedButton->setPosition(glm::vec2(120, buttonPosY));
	nextButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonNextTexture, &simpleTexProgram);
	nextButton->setPosition(glm::vec2(220, buttonPosY));
	nextSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonNextSelectedTexture, &simpleTexProgram);
	nextSelectedButton->setPosition(glm::vec2(220, buttonPosY));
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	if (!simpleText.init("fonts/DroidSerif-Bold.ttf")) {
		cout << "Couldn't load font" << endl;
	}

	int levelNum = TextProcessor::instance().levelNumber;
	if (levelNum == 1) newPath = "maps/Level2.txt";
	else if (levelNum == 2) newPath = "maps/Level3.txt";
	else if (levelNum == 3) newPath = "maps/Level4.txt";
	else newPath = "maps/Level1.txt";
}

void EndScene::update(int deltaTime) {
	currentTime += deltaTime;
	selected = checkButtonsColision();

	if (Game::instance().getLeftMousePressed()) {
		if (selected == MENU_BUTTON) state = MENU_CHOSEN;
		else if (selected = NEXT_BUTTON) state = NEXT_CHOSEN;
		else if (selected == RETRY_BUTTON) state = RETRY_CHOSEN;
	}
}


Scene * EndScene::changeState() {
	switch (state) {
	case MENU_CHOSEN: {
		Scene * menu = new Menu();
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		menu->init();
		return menu;
	}
	case NEXT_CHOSEN: {
		Scene * scene = new StartScene(newPath);
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}
	case RETRY_CHOSEN: {
		Scene* scene = new PlayScene(path);
		AudioEngine::instance().buttonEffect();
		scene->init();
		return scene;
	}
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
	simpleText.render(saved, glm::vec2(340, 200), 48, colorGreen);

	// Required
	string required = "Required: " + to_string(numLemmingsMin);
	simpleText.render(required, glm::vec2(340, 260), 48, colorGreen);

	// Message
	string message = "";
	if (numLemmingsExited == numLemmingsTotal) message = "You rescued every lemming on that level";
	else if (numLemmingsExited >= numLemmingsMin) message = "You rescued enough lemmings";
	else if (numLemmingsExited == 0) message = "Oh dear. Not even one poor Lemming saved";
	else message = "You didn't rescue enough lemmings";
	simpleText.render(message, glm::vec2(30, 320), 38, colorGreen);

	simpleTexProgram.use();
	// Continue
	if (selected == MENU_BUTTON)
		menuSelectedButton->render();
	else
		menuButton->render();

	if (selected == RETRY_BUTTON)
		retrySelectedButton->render();
	else
		retryButton->render();
	if (win) {
		if (selected == NEXT_BUTTON)
			nextSelectedButton->render();
		else
			nextButton->render();
	}

}

EndScene::EndButton EndScene::checkButtonsColision() {
	int mouseX, mouseY;
	Game::instance().getMousePosition(mouseX, mouseY);
	if (mouseY >= buttonPosY * 3 && mouseY <= (buttonPosY + buttonSizeY) * 3) {
		if ((mouseX >= (20 * 3) && mouseX <= ((20 + buttonSizeX) * 3)))
			return MENU_BUTTON;
		else if ((mouseX >= (220 * 3) && mouseX <= ((220 + buttonSizeX) * 3)))
			return NEXT_BUTTON;
		else if ((mouseX >= (120 * 3) && mouseX <= ((120 + buttonSizeX) * 3)))
			return RETRY_BUTTON;

	}
	return NONE_BUTTON;
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
