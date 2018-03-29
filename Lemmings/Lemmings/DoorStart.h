#ifndef _DOORSTART_INCLUDE
#define _DOORSTART_INCLUDE

#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class DoorStart {

public:
	DoorStart(int color);
	~DoorStart();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet);
	void update(int deltaTime);
	void render();
	glm::vec2 getPosition();

	

	enum StartColors {
		BROWN,
		GRAY
	};

private:
	Sprite* sprite;

	enum StartAnims {
		CLOSED,
		OPENING,
		OPEN
	};

	
	float xSpriteSheet, currentTime;


	void open();
	void setAnimations();
};

#endif

