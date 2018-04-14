#include "Menu.h"


Menu::Menu() {
}


Menu::~Menu() {
}

void Menu::init() {
	AudioEngine::instance().playMusic("Music/menu.mp3");
	Cursor::instance().lemmingInside(false);
	selected = NONE_BUTTON;
	state = ON;
	currentTime = 0.f;

	initShaders();

	loadTextures();
	createSprites();

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	selected = checkButtonsColison();
	if (Game::instance().getLeftMousePressed()) {
		if (selected == EXIT_BUTTON) state = EXIT_CHOSEN;
		else if (selected == PLAY_BUTTON) state = PLAY_CHOSEN;
		else if (selected == INSTRUCTIONS_BUTTON) state = INSTRUCTIONS_CHOSEN;
		else if (selected == CREDITS_BUTTON) state = CREDITS_CHOSEN;
	}
}

Scene* Menu::changeState() {
	switch (state) {
	case PLAY_CHOSEN: {
		Scene * scene = new StartScene("maps/Level1.txt");
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().yippee();
		AudioEngine::instance().playMusic("Music/lemmings.wav");
		scene->init();
		return scene;
	}
	case INSTRUCTIONS_CHOSEN: {
		Scene * instructions = new InstructionsScene();
		AudioEngine::instance().buttonEffect();
		instructions->init();
		return instructions;
	}
	case CREDITS_CHOSEN: {
		Scene * credits = new Credits();
		AudioEngine::instance().buttonEffect();
		credits->init();
		return credits;
	}
	case EXIT_CHOSEN:
		Game::instance().closeGame();
		break;

	default:
		break;
	}
	return this;
}

void Menu::render() {
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	background->render();
	title->render();
			
	simpleTexProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.0f);
	


	if (selected == PLAY_BUTTON)
		playSelectedButton->render();
	else
		playButton->render();

	if (selected == INSTRUCTIONS_BUTTON)
		instructionsSelectedButton->render();
	else
		instructionsButton->render();

	if (selected == CREDITS_BUTTON)
		creditsSelectedButton->render();
	else
		creditsButton->render();

	if (selected == EXIT_BUTTON)
		exitSelectedButton->render();
	else
		exitButton->render();

}

Menu::MenuButton Menu::checkButtonsColison() {
	int mouseX, mouseY;
	Game::instance().getMousePosition(mouseX, mouseY);
	if (mouseX >= buttonPosX * 3 && mouseX <= (buttonPosX + buttonSizeX) * 3) {
		if ((mouseY >= (80 * 3) && mouseY <= ((80 + buttonSizeY) * 3))) 
			return PLAY_BUTTON;
		else if ((mouseY >= (112 * 3) && mouseY <= ((112 + buttonSizeY) * 3)))
			return INSTRUCTIONS_BUTTON;
		else if ((mouseY >= (144 * 3) && mouseY <= ((144 + buttonSizeY) * 3)))
			return CREDITS_BUTTON;
		else if ((mouseY >= (173 * 3) && mouseY <= ((173 + buttonSizeY) * 3)))
			return EXIT_BUTTON;
	}
	return NONE_BUTTON;
}

void Menu::loadTextures() {
	titleTexture.loadFromFile("images/logo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);

	bgTexture.loadFromFile("images/backTexture.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);

	buttonPlayTexture.loadFromFile("images/Button_Big_Play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonPlayTexture.setMinFilter(GL_NEAREST);
	buttonPlayTexture.setMagFilter(GL_NEAREST);
	buttonPlaySelectedTexture.loadFromFile("images/Button_Big_Play_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonPlaySelectedTexture.setMinFilter(GL_NEAREST);
	buttonPlaySelectedTexture.setMagFilter(GL_NEAREST);

	buttonInstructionsTexture.loadFromFile("images/Button_Big_Instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonInstructionsTexture.setMinFilter(GL_NEAREST);
	buttonInstructionsTexture.setMagFilter(GL_NEAREST);
	buttonInstructionsSelectedTexture.loadFromFile("images/Button_Big_Instructions_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonInstructionsSelectedTexture.setMinFilter(GL_NEAREST);
	buttonInstructionsSelectedTexture.setMagFilter(GL_NEAREST);

	buttonCreditsTexture.loadFromFile("images/Button_Big_Credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonCreditsTexture.setMinFilter(GL_NEAREST);
	buttonCreditsTexture.setMagFilter(GL_NEAREST);
	buttonCreditsSelectedTexture.loadFromFile("images/Button_Big_Credits_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonCreditsSelectedTexture.setMinFilter(GL_NEAREST);
	buttonCreditsSelectedTexture.setMagFilter(GL_NEAREST);

	buttonExitTexture.loadFromFile("images/Button_Big_Exit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonExitTexture.setMinFilter(GL_NEAREST);
	buttonExitTexture.setMagFilter(GL_NEAREST);
	buttonExitSelectedTexture.loadFromFile("images/Button_Big_Exit_Selected.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonExitSelectedTexture.setMinFilter(GL_NEAREST);
	buttonExitSelectedTexture.setMagFilter(GL_NEAREST);

}

void Menu::createSprites() {
	title = Sprite::createSprite(glm::vec2(float((446)*0.50f), float((154)*0.50f)), glm::vec2(1.f, 1.f), &titleTexture, &simpleTexProgram);
	title->setPosition(glm::vec2(float((CAMERA_WIDTH / 2) - 446 / 4), 0.0f));

	background = Sprite::createSprite(glm::vec2(1920.f / 5.15f, 1080.f / 5.15f), glm::vec2(1.f, 1.f),
		&bgTexture, &simpleTexProgram);
	background->setPosition(background->position() + glm::vec2(-20, 0));

	buttonPosX = (CAMERA_WIDTH / 2) - 420 / 6; // x = 90
	buttonSizeX = 420 / 3; // x = 140;
	buttonSizeY = 22; // because 65/3 is not exact...
	playButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonPlayTexture, &simpleTexProgram);
	playButton->setPosition(glm::vec2(buttonPosX, 80));
	playSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonPlaySelectedTexture, &simpleTexProgram);
	playSelectedButton->setPosition(glm::vec2(buttonPosX, 80));
	instructionsButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonInstructionsTexture, &simpleTexProgram);
	instructionsButton->setPosition(glm::vec2(buttonPosX, 112));
	instructionsSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonInstructionsSelectedTexture, &simpleTexProgram);
	instructionsSelectedButton->setPosition(glm::vec2(buttonPosX, 112));
	creditsButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonCreditsTexture, &simpleTexProgram);
	creditsButton->setPosition(glm::vec2(buttonPosX, 144));
	creditsSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonCreditsSelectedTexture, &simpleTexProgram);
	creditsSelectedButton->setPosition(glm::vec2(buttonPosX, 144));
	exitButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonExitTexture, &simpleTexProgram);
	exitButton->setPosition(glm::vec2(buttonPosX, 176));
	exitSelectedButton = Sprite::createSprite(glm::vec2(buttonSizeX, buttonSizeY), glm::vec2(1.f, 1.f), &buttonExitSelectedTexture, &simpleTexProgram);
	exitSelectedButton->setPosition(glm::vec2(buttonPosX, 176));
}

void Menu::initShaders() {
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
