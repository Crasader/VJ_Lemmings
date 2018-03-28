#ifndef _PLAYSCENE_INCLUDE
#define _PLAYSCENE_INCLUDE

#include "Scene.h"
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Game.h"
#include "Lemming.h"
#include "EntityManager.h"
#include "GUI.h"

class PlayScene :
	public Scene
{
public:
	PlayScene();
	~PlayScene();
	void init() ;
	void render();
	void update(int deltaTime);
	Scene* changeState();

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void effectForLemming(int mouseX, int mouseY, int effect);


private:
	Texture colorTexture;
	MaskedTexturedQuad * map;
	VariableTexture maskTexture;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	bool bExit, bMouseLeft, bMouseRight, bMoveCameraRight, bMoveCameraLeft;
	bool bBasher, bBlocker, bDigger, bBuilder, bClimber;
	float cameraX, cameraY;
	EntityManager* manager;
	GUI * gui;
	
};

#endif

