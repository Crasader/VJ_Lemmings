#include "PlayScene.h"

#include "Cursor.h"



PlayScene::PlayScene(string levelPath) {
	map = NULL;
	path = levelPath;
}

PlayScene::~PlayScene() {
	if (map != NULL) {
		delete map;
	}
}

void PlayScene::init() {
	state = ON;
	currentTime = 0.0f;
	armageddon = doubleSpeed = false;
	count = 300;
	pause = false;
	initShaders();

	getLevelData();
	createMap();

	manager = new EntityManager(TextProcessor::instance().lemmings, simpleTexProgram, &colorTexture, &maskTexture);
	manager->init();
	manager->setStartDoor(TextProcessor::instance().startDoor, TextProcessor::instance().doorStartColor);
	manager->setEndDoor(TextProcessor::instance().endDoor, TextProcessor::instance().doorEndColor);
	AudioEngine::instance().playMusic(&TextProcessor::instance().musicPath[0]);
	if (TextProcessor::instance().isBomb)
		manager->setBomb(TextProcessor::instance().bombPos);
	if (TextProcessor::instance().isPortal)
		manager->setPortals(TextProcessor::instance().bluePos, TextProcessor::instance().orangePos);
	


	gui = new InterfazUsuario();
	setGUI();
	gui->init(colorTexture, maskTexture, cameraX, cameraY);

	projection = glm::ortho(cameraX, cameraX + float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	
}

void PlayScene::update(int deltaTime) {
	if (doubleSpeed) deltaTime = deltaTime * 2;
	else if (pause) deltaTime = 0;
	currentTime += deltaTime;
	timeLeft = maxTime - (currentTime / 1000);

	if (timeLeft < 0) state = END;
	int lemmingsStillAlive = TextProcessor::instance().lemmings - manager->getLemmingsDied() - manager->getLemmingsExited();
	if (lemmingsStillAlive == 0) state = END;
	if (armageddon) {
		count = 0;
		state = END;
		armageddon = false;
	}
	if (Game::instance().getKey(27)) state = EXIT_CHOSEN;

	numBombers = manager->getBombersAmount();
	if (numBombers > 0) 
		setGUI();
	

	// change cursor if mouse selects a lemmings
	int x = 0, y = 0;
	Game::instance().getMousePosition(x, y);
	if (manager->lemmingInCursor(cameraX + x/3, cameraY + y/3))
		Cursor::instance().lemmingInside(true);
	else 
		Cursor::instance().lemmingInside(false);

	// mouse near borders -> update camera
	if (x > 900 && y < 495 && cameraX < (TextProcessor::instance().width - CAMERA_WIDTH))
		cameraX += 2;
	else if (x < 60 && y < 495 && cameraX > 0)
		cameraX -= 2;
	projection = glm::ortho(0.f + cameraX, float(CAMERA_WIDTH - 1) + cameraX, float(CAMERA_HEIGHT - 1), 0.f);

	EntityManager::Effect effect = EntityManager::NONE_EFFECT;
	buttonPressed = gui->getButtonPressed();
	if (Game::instance().getLeftMousePressed()) {
		int x = 0, y = 0;
		doubleSpeed = pause = false;
		if (buttonPressed == InterfazUsuario::BASHER_BUTTON)
			effect = EntityManager::BASHER_EFFECT;
		else if (buttonPressed == InterfazUsuario::BLOCKER_BUTTON)
			effect = EntityManager::BLOCKER_EFFECT;
		else if (buttonPressed == InterfazUsuario::CLIMBER_BUTTON)
			effect = EntityManager::CLIMBER_EFFECT;
		else if (buttonPressed == InterfazUsuario::DIGGER_BUTTON)
			effect = EntityManager::DIGGER_EFFECT;
		else if (buttonPressed == InterfazUsuario::FLOATER_BUTTON)
			effect = EntityManager::FLOATER_EFFECT;
		else if (buttonPressed == InterfazUsuario::BOMBER_BUTTON)
			effect = EntityManager::BOMBER_EFFECT;
		else if (buttonPressed == InterfazUsuario::BUILDER_BUTTON)
			effect = EntityManager::BUILDER_EFFECT;
		else if (buttonPressed == InterfazUsuario::DECREASE_BUTTON) {
			manager->increaseSpawnTime();
			gui->decreaseSpawnRate();
		}
		else if (buttonPressed == InterfazUsuario::INCREASE_BUTTON) {
			gui->increaseSpawnRate();
			manager->decreaseSpawnTime();
		}
		else if (buttonPressed == InterfazUsuario::SPEED_BUTTON) {
			doubleSpeed = true;

		}
		else if (buttonPressed == InterfazUsuario::PAUSE_BUTTON)
			pause = true;
		else if (buttonPressed == InterfazUsuario::ARMAGEDDON_BUTTON) {
			armageddon = true;
			manager->killAllLemmings();
		}
		buttonPressed = InterfazUsuario::NONE_BUTTON;


		

	}
	if (effect != EntityManager::NONE_EFFECT)
		if (effectForLemming(x, y, effect))
			setGUI();
	manager->update(deltaTime, buttonPressed,cameraX,cameraY);
	gui->setLemmingsIn(manager->getLemmingsExited());
	gui->update(x/3,  y/3);
	
	gui->setTime(timeLeft);
	gui->setLemmingsOut(manager->getNumLemmingsOut());

}

Scene * PlayScene::changeState() {
	switch (state) {
	case EXIT_CHOSEN: {
		Scene* menu = new Menu();
		AudioEngine::instance().buttonEffect();
		AudioEngine::instance().stopEffect();
		menu->init();
		return menu;
	}
	case END: {
		if (count > 300) {
			Scene* scene = new EndScene(path, manager->getLemmingsExited());
			AudioEngine::instance().buttonEffect();
			AudioEngine::instance().stopEffect();
			scene->init();
			return scene;
		}
		else count++;
		break;
	}
	default:
		break;
	}
	return this;
}

void PlayScene::render() {
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	manager->render();
	
	gui->render();
	
}

bool PlayScene::effectForLemming(int mouseX, int mouseY, EntityManager::Effect effect) {
	int x, y;
	Game::instance().getMousePosition(x, y);
	bool lemmingChanged = false;
	if (effect == EntityManager::DIGGER_EFFECT && numDiggers > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numDiggers--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::BLOCKER_EFFECT && numBlockers > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numBlockers--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::BASHER_EFFECT && numBashers > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numBashers--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::CLIMBER_EFFECT && numClimbers > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numClimbers--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::BUILDER_EFFECT && numBuilders > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numBuilders--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::FLOATER_EFFECT && numFloaters > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numFloaters--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	else if (effect == EntityManager::BOMBER_EFFECT && numBombers > 0) {
		if (manager->clickManager(cameraX + x / 3, cameraY + y / 3, effect)) {
			numBombers--;
			AudioEngine::instance().orderLemmingEffect();
			return true;
		}
	}
	return false;

}

void PlayScene::setGUI() {
	gui->setBashers(numBashers);
	gui->setBlockers(numBlockers);
	gui->setDiggers(numDiggers);
	gui->setClimbers(numClimbers);
	gui->setBuilders(numBuilders);
	gui->setFloaters(numFloaters);
	gui->setBombers(numBombers);
}

void PlayScene::getLevelData() {
	cameraX = TextProcessor::instance().camPos.x;
	cameraY = TextProcessor::instance().camPos.y;
	maxTime = TextProcessor::instance().maxTime;
	numDiggers = TextProcessor::instance().numbDig;
	numBlockers = TextProcessor::instance().numbStop;
	numBashers = TextProcessor::instance().numbBash;
	numFloaters = TextProcessor::instance().numbFlo;
	numClimbers = TextProcessor::instance().numbCli;
	numBombers = TextProcessor::instance().numbBomb;
	numBuilders = TextProcessor::instance().numbBuild;
}

void PlayScene::createMap() {
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(TextProcessor::instance().width), float(TextProcessor::instance().height)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(TextProcessor::instance().path, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(TextProcessor::instance().mPath, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);
}

void PlayScene::initShaders() {
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

