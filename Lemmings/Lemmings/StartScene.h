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
	Texture bgTexture;
	Sprite* background;
	
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	Text simpleText;
	float currentTime;
	string path;

	bool bExit, bContinue;


	void initShaders();

};

