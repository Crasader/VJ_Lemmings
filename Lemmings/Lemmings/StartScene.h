#pragma once
#include "Scene.h"
#include "Menu.h"
#include "PlayScene.h"

class StartScene : public Scene {
public:
	StartScene(string pathLevel);
	~StartScene();
	void update(int deltaTime);
	void render();
	void init();
	Scene* changeState();

private:
	enum StartState {
		ON,
		MENU_CHOSEN,
		OK_CHOSEN
	};
	StartState state;

	enum StartButton {
		MENU_BUTTON,
		OK_BUTTON,
		NONE_BUTTON
	};
	StartButton selected;

	Texture bgTexture, buttonMenuTexture, buttonMenuSelectedTexture, buttonOkTexture, buttonOkSelectedTexture;
	Sprite *background, *menuButton, *menuSelectedButton, *okButton, *okSelectedButton;
	
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	Text simpleText;
	float currentTime;
	string path;

	float buttonPosY, buttonSizeX, buttonSizeY;

	StartButton checkButtonsColision();
	void initShaders();

};

