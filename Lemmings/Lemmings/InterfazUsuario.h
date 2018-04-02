#ifndef _INTERFAZUSUARIO_INCLUDE
#define _INTERFAZUSUARIO_INCLUDE

#include <iostream>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Text.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include "Button.h"
#include "Game.h"
#include <vector>
#include <glm/glm.hpp>
#include "AnimKeyframes.h"
#include "AudioEngine.h"
#include "Texture.h"
#include "VariableTexture.h"
#include "MaskedTexturedQuad.h"
#include "Sprite.h"

class VariableTexture;

class InterfazUsuario
{
public:
	InterfazUsuario();
	~InterfazUsuario();
	void init(VariableTexture & colorTexture, VariableTexture & maskTexture,float & camX, float & camY);
	void render();
	void update(int mouseX, int mouseY);
	void setTime(int time);
	void setDiggers(int digger);
	void setClimbers(int climber);
	void setBuilders(int builder);
	void setFloaters(int floater);
	void decreaseSpawnRate();
	void increaseSpawnRate();
	void setBlockers(int bloker);
	void setBashers(int basher);
	void setLemmingsOut(int out);
	void setLemmingsIn(int in);
	void setBombers(int bomber);
	int getButtonPressed();
private:

	int time;
	int out;
	int in;
	int digger;
	int basher;
	int climber;
	int floater;
	int spawnRate;
	int buttonSelected;
	int mouseX;
	int mouseY;
	int blocker;
	int bomber;
	int builder;
	float  * camX;
	float * camY;
	Texture frame, selector;
	VariableTexture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad* map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	Text info;
	glm::mat4 projection;
	vector<Button*> buttons;
	Sprite * marco;
	Sprite * camSelector;
	float scaleFactor;
	void placeButtons();
	void renderButtons();
	void initShader();
	bool checkColisionMinimap(int mouseX, int mouseY);
};
#endif

