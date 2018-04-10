#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


class Portal {
public:

	enum PortalColors {
		BLUE,
		ORANGE
	};

	Portal(PortalColors color);
	~Portal();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet);
	void update(int deltaTime);
	void render();
	void doubleSpeed();
	void pause();
	void resetSpeed();

	

private: 
	Sprite * sprite;
	float ySpriteSheet, currentTime;

	

};

