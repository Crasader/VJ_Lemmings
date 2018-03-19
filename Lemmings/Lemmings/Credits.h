#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
class Credits :
	public Scene
{
public:
	Credits();
	~Credits();
	void init();
	void render();
	void update(int deltaTime);
	virtual Scene* changeState();
private: 
	Texture bgTexture;
	Sprite* background;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	glm::mat4 projection;
	bool exitToMenu;
	void initShaders();
};

