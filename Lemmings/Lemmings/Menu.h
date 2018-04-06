#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Text.h"

#include "StartScene.h"
#include "InstructionsScene.h"
#include "Credits.h"


class Menu : public Scene {
public:
	Menu();
	~Menu();
	void init();
	void update(int deltaTime);
	virtual Scene* changeState();
	void render();
	
private:
	enum MenuState {
		ON,
		PLAY_CHOSEN,
		EXIT_CHOSEN,
		INSTRUCTIONS_CHOSEN,
		CREDITS_CHOSEN,
	};
	MenuState state;

	enum MenuButton {
		PLAY_BUTTON,
		INSTRUCTIONS_BUTTON,
		CREDITS_BUTTON,
		EXIT_BUTTON,
		NONE_BUTTON
	};
	MenuButton selected;

	float currentTime;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	Texture titleTexture, bgTexture, 
		buttonPlayTexture, buttonPlaySelectedTexture, buttonInstructionsTexture, buttonInstructionsSelectedTexture,
		buttonCreditsTexture, buttonCreditsSelectedTexture, buttonExitTexture, buttonExitSelectedTexture;
	Sprite *title, *background;
	Sprite *playButton, *instructionsButton, *creditsButton, *exitButton,
		*playSelectedButton, *instructionsSelectedButton, *creditsSelectedButton, *exitSelectedButton;
	float buttonPosX, buttonSizeX, buttonSizeY;

	glm::mat4 projection;
	
	MenuButton checkButtonsColison();

	void loadTextures();
	void createSprites();
	void initShaders();
};

#endif

