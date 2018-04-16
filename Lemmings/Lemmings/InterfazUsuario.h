#ifndef _INTERFAZUSUARIO_INCLUDE
#define _INTERFAZUSUARIO_INCLUDE

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Text.h"
#include "Sprite.h"
#include "Button.h"

#include "AnimKeyframes.h"
#include "AudioEngine.h"
#include "Texture.h"
#include "VariableTexture.h"
#include "MaskedTexturedQuad.h"
#include "Sprite.h"

class InterfazUsuario {
public:
	InterfazUsuario();
	~InterfazUsuario();
	void init(VariableTexture & colorTexture, VariableTexture & maskTexture, int & camX, int & camY);
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

public:
	enum Buttons {
		BASHER_BUTTON,		// 0
		BLOCKER_BUTTON,	
		CLIMBER_BUTTON,		// 2
		DIGGER_BUTTON,
		FLOATER_BUTTON,		// 4
		BOMBER_BUTTON,
		BUILDER_BUTTON,		// 6
		DECREASE_BUTTON,
		INCREASE_BUTTON,	// 8
		SPEED_BUTTON,
		PAUSE_BUTTON,		// 10
		ARMAGEDDON_BUTTON,
		NONE_BUTTON
	};

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
	int  *camX, *camY;
	Texture frame, selector;
	VariableTexture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad* map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	Text info;
	glm::mat4 projection;
	vector<Button*> buttons;
	Sprite *marco, *camSelector;
	float scaleFactor;

private:
	void placeButtons();
	void renderButtons();
	void initShader();
	bool checkColisionMinimap(int mouseX, int mouseY);
};
#endif

