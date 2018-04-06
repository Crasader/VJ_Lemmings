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
	enum EndState {
		ON,
		MENU_CHOSEN,
		NEXT_CHOSEN,
		RETRY_CHOSEN
	};
	EndState state;

	enum EndButton {
		MENU_BUTTON,
		NEXT_BUTTON,
		RETRY_BUTTON,
		NONE_BUTTON
	};
	EndButton selected;

	Texture bgTexture,
		buttonRetryTexture, buttonRetrySelectedTexture, buttonMenuTexture, buttonMenuSelectedTexture, buttonNextTexture, buttonNextSelectedTexture;
	Sprite *background, 
		*retryButton, *retrySelectedButton, *menuButton, *menuSelectedButton, *nextButton, *nextSelectedButton;


	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	Text simpleText;
	bool bExit, bContinue, bRetry;
	float currentTime;

	bool win;
	int numLemmingsExited, numLemmingsMin, numLemmingsTotal;
	string path, newPath;
	float buttonPosY, buttonSizeX, buttonSizeY;

	EndButton checkButtonsColision();
	void initShaders();
};

