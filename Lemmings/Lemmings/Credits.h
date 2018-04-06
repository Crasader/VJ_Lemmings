#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "AudioEngine.h"
class Credits :	public Scene
{
public:
	Credits();
	~Credits();
	void init();
	void render();
	void update(int deltaTime);
	virtual Scene* changeState();
private: 
	enum CreditsState {
		ON,
		MENU_CHOSEN
	};
	CreditsState state;

	enum CreditsButton {
		MENU_BUTTON,
		NONE_BUTTON
	};
	CreditsButton selected;

	Texture bgTexture, titleTexture, buttonMenuTexture, buttonMenuSelectedTexture;
	Sprite *background, *title, *menuButton, *menuSelectedButton;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	Text simpleText;
	
	float buttonSizeY, buttonSizeX;
	
	CreditsButton checkButtonsColision();
	void initShaders();
};

