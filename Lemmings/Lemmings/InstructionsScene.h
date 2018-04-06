#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Credits.h"
#include "AudioEngine.h"

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
	enum InstructionsState {
		ON,
		MENU_CHOSEN
	};
	InstructionsState state;

	enum InstructionsButton {
		MENU_BUTTON,
		NONE_BUTTON
	};
	InstructionsButton selected;
	Texture bgTexture, titleTexture, buttonMenuTexture, buttonMenuSelectedTexture;
	Sprite *background, *menuButton, *menuSelectedButton, *title;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	Text simpleText;
	float buttonSizeY, buttonSizeX;

	InstructionsButton checkButtonsColision();
	void initShaders();
};

