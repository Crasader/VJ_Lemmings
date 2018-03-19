#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"

class InstructionsScene :	public Scene
{
public:
	InstructionsScene();
	~InstructionsScene();
	void init();
	void render();
	void update(int deltaTime);
	virtual Scene* changeState();
private:
	Texture bgTexture, titleTexture;
	Sprite* background;
	Sprite* title;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	bool exitToMenu;
	Text simpleText;
	void initShaders();
};

