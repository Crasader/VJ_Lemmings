#pragma once
#include "Scene.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Text.h"
#include "PlayScene.h"

class Menu :
	public Scene
{
public:
	Menu();
	~Menu();
	void init();
	void render();
	void update(int deltaTime);
	virtual Scene* changeState();
	
private:
	bool bPlay, bExit, bInstructions, bCredits, bUp, bDown,pooledUp,pooledDown;
	glm::vec4 colorGreen, colorWhite;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	Texture titleTexture, bgTexture;
	Sprite* title;
	Sprite* backgrownd;
	glm::mat4 projection;
	Text playText, exitText, instructionsText, creditsText;
	int selected;
	void initShaders();
};

