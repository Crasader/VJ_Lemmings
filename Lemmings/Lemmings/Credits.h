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
	Texture bgTexture, titleTexture;
	Sprite* background;
	Sprite* title;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	bool exitToMenu;
	Text simpleText;
	
	
	void initShaders();
};

