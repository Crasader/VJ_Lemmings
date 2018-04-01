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

class InterfazUsuario
{
public:
	InterfazUsuario();
	~InterfazUsuario();
	void init();
	void render();
	void update(int mouseX, int mouseY);
	void setTime(int time);
	void setDiggers(int digger);
	void setClimbers(int climber);
	void decreaseSpawnRate();
	void increaseSpawnRate();
	void setBlockers(int bloker);
	void setBashers(int basher);
	void setLemmingsOut(int out);
	void setLemmingsIn(int in);
	void setBomber(int bomber);
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
	ShaderProgram simpleTexProgram;
	Text info;
	glm::mat4 projection;
	vector<Button*> buttons;
	void placeButtons();
	void renderButtons();
	void initShader();
};
#endif

