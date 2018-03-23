#pragma once
#include "Scene.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
class StartScene :
	public Scene
{
public:
	StartScene();
	~StartScene();
	void update(int deltaTime);
	void render();
	void init();

private:
	Texture bgTexture, titleTexture;
	Sprite* background;


	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	bool exitToMenu;
	Text simpleText;
	float currentTime;




	void initShaders();

};

