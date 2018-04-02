#pragma once
#include "Scene.h"
#include "StartScene.h"

class EndScene : public Scene
{
public:
	EndScene(string levelPath, int lemmingsExited);
	~EndScene();
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
	bool bExit, bContinue, bRetry;
	float currentTime;

	bool win;
	int numLemmingsExited, numLemmingsMin, numLemmingsTotal;
	string path, newPath;

	void initShaders();
};

